
#ifndef __SLOGGER_H__
#define __SLOGGER_H__

#include <elastos.h>
using namespace Elastos;

namespace Elastos {
namespace Utility {
namespace Logging {
/*!
  @brief 
  @since
  @sa
*/
class Slogger
{
public:
    /*!
     * @brief Send a DEBUG log message.
	 * @param tag
	 * @param msg
     */
    static ECode D(String tag, String msg);

    /*!
     * @brief Send an ERROR log message.
	 * @param tag
	 * @param msg	 
     */
    static ECode E(String tag, String msg);

    /*!
     * @brief Send an INFO log message.
	 * @param tag
	 * @param msg	 
     */
    static ECode I(String tag, String msg);

    /*!
     * @brief Send an VERBOSE log message.
	 * @param tag
	 * @param msg	 
     */
    static ECode V(String tag, String msg);

    /*!
     * @brief Send a WARN log message.
	 * @param tag
	 * @param msg	 
     */
    static ECode W(String tag, String msg);

    /*!
     * @brief Low-level logging call.
	 * @param priority
	 * @param tag
	 * @param msg	 
     */
    static ECode Println(Int32 priority, String tag, String msg);
};

} // Logging
} // Utility
} // Elastos

#endif //__SLOGGER_H__
