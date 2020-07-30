#ifndef CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL
#define CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL

namespace cloudevents {
namespace util {

class BinderUtil {
    public:
        // Constexpr keys used when binding/ unbinding Messages
        inline static constexpr absl::string_view kMetadataPrefix = "ce-";
        inline static constexpr absl::string_view kContenttypePrefix = "application/cloudevents+";
};

} // util
} // cloudevents

#endif //CLOUDEVENTSCPPSDK_V1_UTIL_BINDERUTIL
