/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#pragma once

#include <stdexcept>
#include <string>
#include "FoundationKit/GenericPlatformMacros.h"

NS_FK_BEGIN

/**
 * This is the base class for all exceptions defined
 * in Foundation class library.
 */
class Exception: public std::exception
{
public:
	/** 
	 * Creates an exception.
	 * @param [in] msg        The exception message.
	 * @param [in] code       The exception code.
	 */
	Exception(const std::string& msg, int code = 0);

	/** 
	 * Creates an exception.
	 * @param [in] msg        The exception message.
	 * @param [in] arg        Exception message arg.
	 * @param [in] code       The exception code.
	 */
	Exception(const std::string& msg, const std::string& arg, int code = 0);

	/** 
	 * Creates an exception and stores a clone
	 * of the nested exception.
	 */
	Exception(const std::string& msg, const Exception& nested, int code = 0);


	Exception(const Exception& exc);

	/** 
	 * Destroys the exception and deletes the nested exception.
	 */
	~Exception() throw();

	Exception& operator = (const Exception& exc);

	/** 
	 * Returns a static string describing the exception.
	 */
	virtual const char* name() const throw();

	/** 
	 * Returns the name of the exception class.
	 */	
	virtual const char* className() const throw();

	/** 
	 * Returns a static string describing the exception.
	 */		
	virtual const char* what() const throw();

	/** 
	 * Returns a pointer to the nested exception, or
	 * null if no nested exception exists.
	 */
	const Exception* nested() const;

	/** 
	 * Returns the message text.
	 */	
	const std::string& message() const;

	/** 
	 * Returns the exception code if defined.
	 */			
	int code() const;
	
	/** 
	 * Returns a string consisting of the
	 * message name and the message text.
	 */	
	std::string displayText() const;

	/** 
	 * Creates an exact copy of the exception.
	 * The copy can later be thrown again by
	 * invoking rethrow() on it.
	 */
	virtual Exception* clone() const;

	/** 
	 * (Re)Throws the exception.
	 * This is useful for temporarily storing a
	 * copy of an exception (see clone()), then
	 * throwing it again.
	 */
	virtual void rethrow() const;

protected:
	Exception(int code = 0);
	void message(const std::string& msg);
	void extendedMessage(const std::string& arg);

private:
	std::string _msg;
	Exception*  _pNested;
	int			_code;
};


//
// inlines
//
inline const Exception* Exception::nested() const
{
	return _pNested;
}


inline const std::string& Exception::message() const
{
	return _msg;
}


inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}


inline int Exception::code() const
{
	return _code;
}

/**
 * Macros for quickly declaring and implementing exception classes.
 * Unfortunately, we cannot use a template here because character
 * pointers (which we need for specifying the exception name)
 * are not allowed as template arguments.
 */
#define DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
	class CLS: public BASE														    \
	{																				    \
	public:																			    \
		CLS(int code = CODE);													    	\
		CLS(const std::string& msg, int code = CODE);								    \
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		    \
		CLS(const std::string& msg, const Exception& exc, int code = CODE);	            \
		CLS(const CLS& exc);													        \
		~CLS() throw();																    \
		CLS& operator = (const CLS& exc);											    \
		const char* name() const throw();											    \
		const char* className() const throw();										    \
		Exception* clone() const;                                                       \
		void rethrow() const;                                                           \
	};

#define DECLARE_EXCEPTION(CLS, BASE) \
	DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

#define IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													        \
	CLS::CLS(int code): BASE(code)																	    \
	{																								    \
	}																								    \
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										    \
	{																								    \
	}																								    \
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		    \
	{																								    \
	}																								    \
	CLS::CLS(const std::string& msg, const Exception& exc, int code): BASE(msg, exc, code)	            \
	{																								    \
	}																								    \
	CLS::CLS(const CLS& exc): BASE(exc)																    \
	{																								    \
	}																								    \
	CLS::~CLS() throw()																				    \
	{																								    \
	}																								    \
	CLS& CLS::operator = (const CLS& exc)															    \
	{																								    \
		BASE::operator = (exc);																		    \
		return *this;																				    \
	}																								    \
	const char* CLS::name() const throw()															    \
	{																								    \
		return NAME;																				    \
	}																								    \
	const char* CLS::className() const throw()														    \
	{																								    \
		return typeid(*this).name();																    \
	}																								    \
	Exception* CLS::clone() const															            \
	{																								    \
		return new CLS(*this);																		    \
	}																								    \
	void CLS::rethrow() const																		    \
	{																								    \
		throw *this;																				    \
	}


//
// Standard exception classes
//
DECLARE_EXCEPTION(LogicException, Exception)
DECLARE_EXCEPTION(AssertionViolationException, LogicException)
DECLARE_EXCEPTION(NullPointerException, LogicException)
DECLARE_EXCEPTION(NullValueException, LogicException)
DECLARE_EXCEPTION(BugcheckException, LogicException)
DECLARE_EXCEPTION(InvalidArgumentException, LogicException)
DECLARE_EXCEPTION(NotImplementedException, LogicException)
DECLARE_EXCEPTION(RangeException, LogicException)
DECLARE_EXCEPTION(IllegalStateException, LogicException)
DECLARE_EXCEPTION(InvalidAccessException, LogicException)
DECLARE_EXCEPTION(SignalException, LogicException)
DECLARE_EXCEPTION(UnhandledException, LogicException)

DECLARE_EXCEPTION(RuntimeException, Exception)
DECLARE_EXCEPTION(NotFoundException, RuntimeException)
DECLARE_EXCEPTION(ExistsException, RuntimeException)
DECLARE_EXCEPTION(TimeoutException, RuntimeException)
DECLARE_EXCEPTION(SystemException, RuntimeException)
DECLARE_EXCEPTION(RegularExpressionException, RuntimeException)
DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)
DECLARE_EXCEPTION(NoThreadAvailableException, RuntimeException)
DECLARE_EXCEPTION(PropertyNotSupportedException, RuntimeException)
DECLARE_EXCEPTION(PoolOverflowException, RuntimeException)
DECLARE_EXCEPTION(NoPermissionException, RuntimeException)
DECLARE_EXCEPTION(OutOfMemoryException, RuntimeException)
DECLARE_EXCEPTION(DataException, RuntimeException)
DECLARE_EXCEPTION(InterruptedException, RuntimeException)
DECLARE_EXCEPTION(IndexOutOfBoundsException, RuntimeException)
DECLARE_EXCEPTION(UnsupportedOperationException, RuntimeException)
DECLARE_EXCEPTION(EmptyStackException, RuntimeException)
DECLARE_EXCEPTION(StackOverflowException, RuntimeException)
DECLARE_EXCEPTION(ArithmeticException, RuntimeException)

DECLARE_EXCEPTION(DataFormatException, DataException)
DECLARE_EXCEPTION(SyntaxException, DataException)
DECLARE_EXCEPTION(CircularReferenceException, DataException)
DECLARE_EXCEPTION(PathSyntaxException, SyntaxException)
DECLARE_EXCEPTION(IOException, RuntimeException)
DECLARE_EXCEPTION(ProtocolException, IOException)
DECLARE_EXCEPTION(FileException, IOException)
DECLARE_EXCEPTION(FileExistsException, FileException)
DECLARE_EXCEPTION(FileNotFoundException, FileException)
DECLARE_EXCEPTION(PathNotFoundException, FileException)
DECLARE_EXCEPTION(FileReadOnlyException, FileException)
DECLARE_EXCEPTION(FileAccessDeniedException, FileException)
DECLARE_EXCEPTION(CreateFileException, FileException)
DECLARE_EXCEPTION(OpenFileException, FileException)
DECLARE_EXCEPTION(WriteFileException, FileException)
DECLARE_EXCEPTION(ReadFileException, FileException)
DECLARE_EXCEPTION(UnknownURISchemeException, RuntimeException)

DECLARE_EXCEPTION(ApplicationException, Exception)
DECLARE_EXCEPTION(BadCastException, RuntimeException)

NS_FK_END

