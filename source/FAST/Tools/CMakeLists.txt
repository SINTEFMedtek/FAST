fast_add_subdirectories(
    #OpenIGTLinkClient
    OpenIGTLinkServer
    Pipeline
)
fast_add_sources(
    CommandLineParser.cpp
    CommandLineParser.hpp
)
fast_add_tool(downloadTestData downloadTestData.cpp)