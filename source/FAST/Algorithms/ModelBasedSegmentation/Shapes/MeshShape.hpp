#ifndef MESH_SHAPE_HPP_
#define MESH_SHAPE_HPP_

#include "FAST/Data/Mesh.hpp"

namespace fast {

/**
 * Class used to represent a mesh shape.
 *
 */
class MeshShape {
	public:
		Mesh::pointer getMesh() const;
		void setMesh(Mesh::pointer mesh);
	private:
		Mesh::pointer mMesh;
};

}

#endif
