#include <libconfig.h++>
#include "RaytracerException.hpp"

namespace RayTracer
{
    struct Node;
    typedef std::variant<int, long long, float, std::string, bool> ScalarValue;
    typedef std::unique_ptr<Node> NodePtr;
    typedef std::map<std::string, NodePtr> Object;

    struct Node
    {
        std::variant<ScalarValue, Object> value;
    };

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
        void parseSettings(const libconfig::Setting &setting, RayTracer::Object &result, const std::string prefix);
    };
}