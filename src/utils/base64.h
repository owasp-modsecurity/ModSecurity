

namespace modsecurity {
namespace Utils {

class Base64 {
 public:
    Base64() { }

    static std::string encode(std::string& data);

    static std::string decode(std::string& data, bool forgiven);
    static std::string decode(std::string& data);
    static std::string decode_forgiven(std::string& data);

    static void decode_forgiven_engine(unsigned char *output,
        size_t output_size, size_t *aiming_size, const unsigned char *input,
        size_t input_size);

};


}  // namespace Utils
}  // namespace modsecurity