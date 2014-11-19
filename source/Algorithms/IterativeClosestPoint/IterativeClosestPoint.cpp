#include "IterativeClosestPoint.hpp"
#undef min
#undef max
#include <limits>

namespace fast {

IterativeClosestPoint::IterativeClosestPoint() {
    setInputRequired(0, true);
    setInputRequired(1, true);
    mMaxIterations = 100;
    mMinErrorChange = 0.001;
    mError = -1;
}

void IterativeClosestPoint::setFixedPointSet(
        const PointSet::pointer fixedSet) {
    setInputData(0, fixedSet);
}

void IterativeClosestPoint::setMovingPointSet(
        const PointSet::pointer movingSet) {
    setInputData(1, movingSet);
}

LinearTransformation IterativeClosestPoint::getOutputTransformation() {
    return mTransformation;
}

float IterativeClosestPoint::getError() const {
    return mError;
}

/**
 * Create a new matrix which is matrix A rearranged.
 * This matrix has the same size as B
 */
MatrixXf rearrangeMatrixToClosestPoints(const MatrixXf A, const MatrixXf B) {
    MatrixXf result = MatrixXf::Constant(B.rows(), B.cols(), 0);

    // For each point in B, find the closest point in A
    for(uint b = 0; b < B.cols(); b++) {
        Vector3f pointInB = B.col(b);
        float minDistance = std::numeric_limits<float>::max();
        uint closestPoint = 0;
        for(uint a = 0; a < A.cols(); a++) {
            Vector3f pointInA = A.col(a);
            float distance = (pointInA-pointInB).norm();
            if(distance < minDistance) {
                minDistance = distance;
                closestPoint = a;
            }
        }
        result.col(b) = A.col(closestPoint);
    }

    return result;

}

/*
 * Get centroid
 */
Vector3f getCentroid(const MatrixXf m) {
    return m.rowwise().sum() / m.cols();
}

void IterativeClosestPoint::execute() {
    float error = std::numeric_limits<float>::max(), previousError;
    uint iterations = 0;
    Eigen::Transform<float, 3, Eigen::Affine> currentTransformation =
            Eigen::Transform<float, 3, Eigen::Affine>::Identity(); // TODO should get current from scene graph..

    // Get access to the two point sets
    PointSetAccess accessFixedSet = ((PointSet::pointer)getInputData(0))->getAccess(ACCESS_READ);
    PointSetAccess accessMovingSet = ((PointSet::pointer)getInputData(1))->getAccess(ACCESS_READ);

    // These matrices are Nx3
    MatrixXf fixedPoints = accessFixedSet.getPointSetAsMatrix();
    MatrixXf movingPoints = accessMovingSet.getPointSetAsMatrix();

    // Want to choose the smallest one as moving
    bool invertTransform = false;
    if(fixedPoints.cols() < movingPoints.cols()) {
        MatrixXf temp = fixedPoints;
        fixedPoints = movingPoints;
        movingPoints = temp;
        invertTransform = true;
    }
    do {
        previousError = error;
        MatrixXf movedPoints = currentTransformation*(movingPoints.colwise().homogeneous());

        // Match closest points using current transformation
        MatrixXf rearrangedFixedPoints = rearrangeMatrixToClosestPoints(
                fixedPoints, movedPoints);

        // Get centroids
        Vector3f centroidFixed = getCentroid(rearrangedFixedPoints);
        Vector3f centroidMoving = getCentroid(movedPoints);

        // Create correlation matrix H
        MatrixXf H = (movedPoints.colwise()-centroidMoving)*
                (rearrangedFixedPoints.colwise()-centroidFixed).transpose();

        // Do SVD on H
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);

        // Estimate rotation as R=V*U.transpose()
        Matrix3f R = svd.matrixV()*svd.matrixU().transpose();

        // Estimate translation as t = centroid fixed - R*centroid moving
        //Vector3f T = centroidFixed - R*centroidMoving;
        Vector3f T = centroidFixed - centroidMoving;

        // Update current transformation
        Eigen::Transform<float,3,Eigen::Affine> rotationTransform = Eigen::Transform<float,3,Eigen::Affine>::Identity();
        //rotationTransform.linear() = R;
        rotationTransform.translation() = T;
        currentTransformation = rotationTransform*currentTransformation;

        // Calculate RMS error
        MatrixXf distance = rearrangedFixedPoints - currentTransformation*(movingPoints.colwise().homogeneous());
        error = 0;
        for(uint i = 0; i < distance.cols(); i++) {
            error += pow(distance.col(i).norm(),2);
        }
        error = sqrt(error / distance.cols());

        iterations++;
        std::cout << "Error: " << error << std::endl;
        // To continue, change in error has to be above min error change and nr of iterations less than max iterations
    } while(previousError-error > mMinErrorChange && iterations < mMaxIterations);
    if(invertTransform)
        currentTransformation = currentTransformation.inverse();
    mError = error;
    std::cout << "Finished after " << iterations << " iterations" << std::endl;
    std::cout << "Final transform:" << std::endl;
    std::cout << currentTransformation.affine() << std::endl;
    mTransformation.setTransform(currentTransformation);
}


}
