

namespace modsecurity {
namespace Utils {

class Base64 {
 public:
    Base64() { }

    static std::string encode(std::string& data);
    static std::string decode(std::string& data);
};


}  // namespace Utils
}  // namespace modsecurity