


#ifndef FORMAT_OVERLOADS_H
#define FORMAT_OVERLOADS_H

#include<string>
#include<sstream>

namespace std{

    template <>
    struct formatter<gpp::vector3d> {
        // Especificação de formato
        auto format(const gpp::vector3d& st, format_context& ctx)const {
            return format_to(ctx.out(), "{:.2f}, {:.2f}, {:.2f}", st.x, st.y, st.z);
        }
        auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }
    };

    template <>
    struct formatter<gpp::quaternion> {
        // Especificação de formato
        auto format(const gpp::quaternion& st, format_context& ctx)const {
            return format_to(ctx.out(), "{:.2f}, {:.2f}, {:.2f}, {:.2f}", st.x, st.y, st.z, st.w);
        }
        auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }
    };

}

inline std::string FMT_VEC(const gpp::vector3d& v1, const gpp::vector3d& v2) {
    std::string s = "XYZ";
    std::stringstream ss;

    ss << std::fixed;
    ss.precision(2);

    for (unsigned int i = 0; i < 3; i++) {
        ss << s[i] << ": " << v1[i] << ", " << v2[i] << std::endl;
    }
    return ss.str();
}

#endif
