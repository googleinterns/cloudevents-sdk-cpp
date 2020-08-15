#include "formatter_util.h"

namespace cloudevents {
namespace formatter_util {

using ::cloudevents::format::Format;
using ::cloudevents::format::Formatter;
using ::cloudevents::format::JsonFormatter;

constexpr char kErrUnkFormat[] = "A Format has not been handled.";
constexpr char kErrUnkFormatStr[] = "The given format is not recognized by the SDK.";
constexpr char kErrUnkFormatter[] = "Could not find formatter for given format.";

}  // namespace formatter_util
}  // namespace cloudevents
