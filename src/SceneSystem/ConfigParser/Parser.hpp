#pragma once
#include <libconfig.h++>
#include <variant>
#include <map>
#include <vector>
#include <memory>
#include "../../Objects/Plugin.hpp"

namespace RayTracer
{
    class Parser
    {
    private:
        libconfig::Config _cfg;
        libconfig::Setting *_root;

    public:
        Parser(const std::string &filename);
        ~Parser();

        NodePtr _node;
        void getValue(const libconfig::Setting &s, ScalarValue &result);
        void insert_value(Object &obj, const std::string &prefix, const libconfig::Setting &s);
        void insert_vector_object(Object &obj, const std::string &name, const libconfig::Setting &s);
        void parseSettings(const libconfig::Setting &setting, RayTracer::Object &result, const std::string prefix);
    };
}