
#ifndef __STRINGBUFFER_H__
#define __STRINGBUFFER_H__

#include <elastos.h>
using namespace Elastos;

namespace Elastos {
namespace Core {
/*!
	@class StringBuffer
	@brief A string buffer is like a String, but can be modified. At any point in time it contains
	some particular sequence of characters, but the length and content of the sequence can be changed
	through certain method calls.

	The principal operations on a StringBuffer are the overloaded operator methods, which are overloaded
	so as to accept data of some type. Each effectively converts a given datum to a string and then appends
	the characters of that string to the string buffer. The addition operator method always adds these
	characters at the end of the buffer;
	Every string buffer has a capacity. As long as the length of the character sequence contained in the
	string buffer does not exceed the capacity, it is not necessary to allocate a new internal buffer array.
	If the internal buffer overflows, it is automatically made larger.
*/
class StringBuffer
{
public:
	/*!
		@brief Constructs a String buffer that contains the same characters as the specified
		       @c const @c char* .

		@param  A constant character pointer.
	*/
    StringBuffer(const char* = NULL);
    /*!
		@brief Constructs a String buffer initialized to the contents of the specified @c String.

		@param  The initial contents of the buffer.
	*/
    StringBuffer(const String&);
	 /*!
		@brief A copied constructor.Copies a String buffer's reference from a String buffer's object.

		Its function is to copy a String buffer object's members into another String buffer object's
		corresponding members.
		@param  A reference to a constant StringBuffer itself.
	*/
    StringBuffer(const StringBuffer&);

    StringBuffer(Int32 capacity);
	/*!
		@brief A destructor.
	*/
    ~StringBuffer();
    /*!
		@brief Overloads the assignment operator.

		Returns a constant String buffer that is assigned by the contents of the specified @c const
		@c StringBuffer @c & .
		@param  A constant reference to @c StringBuffer.
		@return The object of a constant reference to StringBuffer.
	*/
    StringBuffer& operator=(const StringBuffer&);
	/*!
		@brief Overloads the addition operator with an argument of the constant reference to StringBuffer.

		Returns a constant String buffer that is assigned by the current StringBuffer object itself
		plus the specifified @c const @c StringBuffer& object.
		@param  A constant reference to @c StringBuffer.
		@return The object of a constant reference to StringBuffer.
	*/
    StringBuffer& operator+=(const StringBuffer&);
    StringBuffer& operator+=(const String&);
    /*!
        @brief Overloads the addition operator with an argument of the constant reference to Int32.

        Returns a constant String buffer object that is assigned by the current StringBuffer object
        itself plus the specified @c const @c Int32& .
        @param  A constant reference to @c Int32.
        @return The object of a constant reference to StringBuffer.
    */
    StringBuffer& operator+=(const Char32);
	/*!
		@brief Overloads the addition operator with an argument of the constant reference to Int32.

		Returns a constant String buffer object that is assigned by the current StringBuffer object
		itself plus the specified @c const @c Int32& .
		@param  A constant reference to @c Int32.
		@return The object of a constant reference to StringBuffer.
	*/
    StringBuffer& operator+=(const Int32);
    /*!
		@brief Overloads the addition operator with an argument of the constant reference to Int64.

		Returns a constant String buffer object that is assigned by the current StringBuffer object
		itself plus the specified @c const @c Int64& .
		@param  A constant reference to @c Int64.
		@return The object of a constant reference to StringBuffer.
	*/
    StringBuffer& operator+=(const Int64);

	/*!
		@brief Overloads the addition operator with an argument of the constant reference to Boolean.

		Returns a constant String buffer object that is assigned by the current StringBuffer object
		itself plus the specified @c const @c Boolean& .
		@param  A constant reference to @c Boolean.
		@return The object of a constant reference to StringBuffer.
	*/
    StringBuffer& operator+=(const Boolean);
    /*!
		@brief Overloads the addition operator with an argument of the constant reference to Address.

		Returns a constant String buffer object that is assigned by the current StringBuffer object
		itself plus the specified @c const @c Address& .
		@param  A constant reference to @c Address.
		@return The object of a constant reference to StringBuffer.
	*/
	//UTF-8 string
	StringBuffer& operator+=(const char*);
    StringBuffer& operator+=(const void*);
	/*!
		@brief A conversion operator that allows the object to be explicitly or implicitly casted to @c String.

		When such a cast occurs,the operator is invoked and the result of the cast is the result of the invocation.
	*/
    operator CString() const;
	/*!
		@brief A conversion operator that allows the object to be explicitly or implicitly casted to @c const @c char* .

		When such a cast occurs,the operator is invoked and the result of the cast is the result of the invocation.
	*/
    operator const char*() const;
    /*!
	    @brief See if the current string buffer is empty,if it is empty then return true,else return false.

		@return a bool value.
	*/
    Boolean IsNull() const;

	/*!
	    @brief Returns the length of the current StringBuffer object.
		@return an Int32 value representing the length of the StringBuffer object.
	*/
    Int32 GetLength();
    /*!
		@brief Returns a substring which starts from the index @a start to the index @a end of the current StringBuffer
		object.NOTE:When you don't need to use the substring again,please go to free the space of this substring by
		yourself.
		@param start the starting index to generate the substring.
		@param end the ending index to generate the substring.
	*/
    String Substring(Int32 start, Int32 end);

private:
    /*!
	    @brief constracts StringBuffer with s.

		Constract a string buffer with a constant characters in it and the specified initial step.
		@param s A constant character pointer argument ,it contains the initial contents of the string buffer.
	*/
    void Init(const char* s, Int32 step);
    /*!
	    @brief Increases the capacity of the string buffer.
		@param size An integer value representing the size want to increase for the string buffer.
	*/
    void Break(Int32 size);

private:
	/*!
	    @var mLength
		@brief The number of bits used to represent the length of a StringBuffer object.
	*/
    Int32   mLength;
	/*!
		@var mString
		@brief The character pointer used to contain a string value.
	*/
    char*  mString;
	/*!
		@var mStep
		@brief The Integer value used to define a step how long a string buffer would increase for one time.
	*/
    Int32   mStep;
	/*!
		@var mCapacity
		@brief Integer value used to define the capacity of a string buffer.
	*/
    Int32   mCapacity;
};

} // Core
} // Elastos

using namespace Elastos::Core;
	/*!
		@brief Overloads the addition operator between two string buffer object.
		@param  A constant reference to @c StringBuffer.
		@param  A constant reference to @c StringBuffer.
		@return A StringBuffer object.
	*/
StringBuffer operator+(const StringBuffer&, const StringBuffer&);
StringBuffer operator+(const StringBuffer&, const String&);
    /*!
		@brief Overloads the addition operator between a string buffer object and an 32 bits integer value.
		@param  A constant reference to @c StringBuffer.
		@param  A integer value of 32 bits.
		@return A StringBuffer object.
	*/
StringBuffer operator+(const StringBuffer&, const Char32);
	/*!
		@brief Overloads the addition operator between a string buffer object and an 32 bits integer value.
		@param  A constant reference to @c StringBuffer.
		@param  A integer value of 32 bits.
		@return A StringBuffer object.
	*/
StringBuffer operator+(const StringBuffer&, const Int32);
    /*!
		@brief Overloads the addition operator between a string buffer object and an 64 bits integer value.
		@param  A constant reference to @c StringBuffer.
		@param  A integer value of 64 bits.
		@return A StringBuffer object.
	*/
StringBuffer operator+(const StringBuffer&, const Int64);
	/*!
		@brief Overloads the addition operator between a string buffer object and a boolean value.
		@param  A constant reference to @c StringBuffer.
		@param  A boolean value.
		@return A StringBuffer object.
	*/
StringBuffer operator+(const StringBuffer&, const Boolean);
StringBuffer operator+(const StringBuffer&, const char*);
StringBuffer operator+(const StringBuffer&, const void*);

#endif //__STRINGBUFFER_H__

