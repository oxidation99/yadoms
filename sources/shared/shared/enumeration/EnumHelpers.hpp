#pragma once

#include "IExtendedEnum.h"
#include "shared/exception/Exception.hpp"
#include "shared/exception/OutOfRange.hpp"
/*

This class contains macros for defining extended macros which add the ability to use string and/or values for enum

Documentation : https://github.com/Yadoms/yadoms/wiki/Enhanced%20enumerations/


//////////////////////////////////////////////
//MACRO usage
//////////////////////////////////////////////

//////////////////////////////////////////
//The header part
//////////////////////////////////////////
DECLARE_ENUM_HEADER(ECurtainCommand,
((Stop))
((Open))
((Close))
);

//enum with custom integers
DECLARE_ENUM_HEADER(ECurtainCommand,
((Stop)(0))
((Open)(8))
((Close)(-2))
);


//shared enum with automatically generated integers
DECLARE_ENUM_HEADER_SHARED(ECurtainCommand, YADOMS_SHARED_EXPORT,
((Stop))
((Open))
((Close))
);

//shared enum with custom integers
DECLARE_ENUM_HEADER_SHARED(ECurtainCommand, YADOMS_SHARED_EXPORT,
((Stop)(0))
((Open)(8))
((Close)(-2))
);

//////////////////////////////////////////
//The implementation part
//////////////////////////////////////////

//classic implementation, each string is generated by the name (Stop -> "stop")
DECLARE_ENUM_IMPLEMENTATION(ECurtainCommand,
((Stop))
((Open))
((Close))
);

//specific strings
DECLARE_ENUM_IMPLEMENTATION(ECurtainCommand,
((Stop)("stop_enum")
((Open)("another_open")
((Close)("exit")
);

//nested enum
DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::ECurtainCommand, ECurtainCommand,
((Stop))
((Open))
((Close))
);

//nested enum with specific strings
DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::ECurtainCommand, ECurtainCommand,
((Stop)("stop_enum")
((Open)("another_open")
((Close)("exit")
);




//////////////////////////////////////////////
//MACRO expansion
//////////////////////////////////////////////

//////////////////////////////////////////
//The header part
//////////////////////////////////////////
class __declspec(dllexport) ECurtainCommand: public shared::enumeration::IExtendedEnum {
public:
enum domain {
kStopValue = 0,
kOpenValue = 1,
kCloseValue = 2,
};

static const ECurtainCommand kStop;
static const ECurtainCommand kOpen;
static const ECurtainCommand kClose;

ECurtainCommand();
ECurtainCommand(const ECurtainCommand & valueTocopy);
ECurtainCommand(const std::string & valueAsString);
ECurtainCommand(const char * valueAsString);
ECurtainCommand(const int valueAsInt);
virtual ~ECurtainCommand();
operator int() const;
operator std::string() const;
ECurtainCommand & operator=(int const& obj);
ECurtainCommand & operator=(std::string const& obj);
ECurtainCommand & operator=(const char * obj);
ECurtainCommand & operator=(const ECurtainCommand & obj);
ECurtainCommand const operator() () const;
const std::string & toString() const;
int toInteger() const;
virtual void fromString(const std::string & val);
static ECurtainCommand parse(const std::string & val);
static bool isDefined(const std::string & stringValue);
static bool isDefined(const int intValue);
private:
int m_value;
static const std::string StopString;
static const std::string OpenString;
static const std::string CloseString;
};

//////////////////////////////////////////
//The header part with SHARED
//////////////////////////////////////////
//same as above; with export
class __declspec(dllexport) ECommand: public shared::enumeration::IExtendedEnum.....





//////////////////////////////////////////
//The implementation part
//////////////////////////////////////////

const ECurtainCommand  ECurtainCommand ::kStop( ECurtainCommand ::kStopValue );
const ECurtainCommand  ECurtainCommand ::kOpen( ECurtainCommand ::kOpenValue );
const ECurtainCommand  ECurtainCommand ::kClose( ECurtainCommand ::kCloseValue );

const std::string ECurtainCommand::StopString = std::string("Stop");
const std::string ECurtainCommand::OpenString = std::string("Open");
const std::string ECurtainCommand::CloseString = std::string("Close");

ECurtainCommand::ECurtainCommand() : m_value( kStop )
{
if(!isDefined(m_value))
throw shared::exception::COutOfRange("Invalid enum value");
}

ECurtainCommand::ECurtainCommand(const int value)
: m_value(value)
{
if(!isDefined(m_value))
throw shared::exception::COutOfRange("Invalid enum value");
}

ECurtainCommand::ECurtainCommand(const std::string & val)
: m_value(parse(val).m_value)
{
}

ECurtainCommand::ECurtainCommand(const char * val)
: m_value(parse(std::string(val)).m_value)
{
}

ECurtainCommand::ECurtainCommand(const ECurtainCommand & val)
: m_value(val.m_value)
{
if(!isDefined(m_value))
throw shared::exception::COutOfRange("Invalid enum value");
}

ECurtainCommand::~ECurtainCommand()
{
}

ECurtainCommand::operator int() const
{
return m_value;
}

int ECurtainCommand::toInteger() const
{
return m_value;
}

ECurtainCommand::operator std::string() const
{
return toString();
}

ECurtainCommand const ECurtainCommand::operator() () const
{
return m_value;
}

ECurtainCommand & ECurtainCommand::operator=(int const& obj)
{
m_value = obj;
if(!isDefined(m_value))
throw shared::exception::COutOfRange("Invalid enum value");
return *this;
}

ECurtainCommand & ECurtainCommand::operator=(const ECurtainCommand & obj)
{
m_value = obj.m_value;
if(!isDefined(m_value))
throw shared::exception::COutOfRange("Invalid enum value");
return *this;
}

ECurtainCommand & ECurtainCommand::operator=(std::string const& obj)
{
m_value = parse(obj).m_value;
return *this;
}

ECurtainCommand & ECurtainCommand::operator=(const char * obj)
{
m_value = parse(std::string(obj)).m_value;
return *this;
}

const std::string & ECurtainCommand::toString() const
{
switch(m_value)
{
case kStopValue:
return StopString;
case kOpenValue:
return OpenString;
case kCloseValue:
return CloseString;
default :
throw shared::exception::COutOfRange("Invalid enum value");
}
}

void ECurtainCommand::fromString(const std::string & val)
{
if (boost::iequals(val, kStop.toString()))
m_value = kStopValue;
else if (boost::iequals(val, kOpen.toString()))
m_value = kOpenValue;
else if (boost::iequals(val, kClose.toString()))
m_value = kCloseValue;
else
throw shared::exception::COutOfRange(val);
}

ECurtainCommand ECurtainCommand::parse(const std::string & val)
{
ECurtainCommand local;
local.fromString(val);
return local;
}

bool ECurtainCommand::isDefined(const std::string & val)
{
if (boost::iequals(val, kStop.toString()))
return true;
else if (boost::iequals(val, kOpen.toString()))
return true;
else if (boost::iequals(val, kClose.toString()))
return true;
else
return false;
}

bool ECurtainCommand::isDefined(const int val)
{
switch(val)
{
case kStopValue:
case kOpenValue:
case kCloseValue:
return true;
default :
return false;
}



//////////////////////////////////////////
//The implementation part for nested
//////////////////////////////////////////
//same as above, with container class specifier

const std::string CCurtain::ECommand::StopString = std::string("Stop");

CCurtain::ECommand::ECommand()
: m_value( kStop )
{
}


*/




//
/// \brief In the sequence of enum values, the first column is the name
//
#define ENUM_COLUMN_NAME   0

//
/// \brief In the sequence of enum values, the second column is the value
//
#define ENUM_COLUMN_VALUE  1

//
/// \brief In the sequence of enum values, the second column is the value
//
#define ENUM_COLUMN_STRING  1

//
/// \brief Give the real enum name (appending a E before the enumName)
//
#define ENUM_CLASSNAME(_enumName) _enumName

//
/// \brief Give the name of a value (in implementation) : (Off) -> kOff
//
#define ENUM_EXTRACT_NAME_IMPL(_seq) BOOST_PP_CAT(k, _seq)
//
/// \brief Give the variable name of an enum value string  (in implementation): (Off) -> OffString
//
#define ENUM_EXTRACT_CONST_NAME_IMPL(_seq) BOOST_PP_CAT(_seq, String)

//
/// \brief Give the name of a value (in header) : ((Off)(0)) -> kOff
//
#define ENUM_EXTRACT_NAME(_elem) BOOST_PP_CAT(k, BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _elem))


//
/// \brief Give the domain name of a value (in header) : ((Off)(0)) -> kOffValue
//
#define ENUM_EXTRACT_DOMAINNAME(_elem) BOOST_PP_CAT(ENUM_EXTRACT_NAME(_elem), Value)


//
/// \brief Give the name of a value (in header) : ((Off)(0)) -> Off
//
#define ENUM_EXTRACT_NAME_RAW(_seq) BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq)

//
/// \brief Give the value (in header) : ((Off)(0)) -> 0
//
#define ENUM_EXTRACT_VALUE(_seq) BOOST_PP_SEQ_ELEM(ENUM_COLUMN_VALUE, _seq)


//
/// \brief Give the variable name of an enum value string (in header) : ((Off)(0)) -> OffString
//
#define ENUM_EXTRACT_CONST_STRINGNAME(_seq) ENUM_EXTRACT_CONST_NAME_IMPL(BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))

//
/// \brief Give the variable name of an enum value string (in header) : ((Off)(0)) -> kOff
//
#define ENUM_EXTRACT_CONST_NAME(_seq) ENUM_EXTRACT_NAME_IMPL(BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))






//
/// \brief Macro used to get the enum value CONDITION
//
#define DECLARE_VALUE_OR_AUTOINC_CONDITION(_seq) BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(_seq), 2 )

//
/// \brief Macro used to get the enum value TRUE BRANCH
//
#define DECLARE_VALUE_OR_AUTOINC_TRUE_BRANCH(_seq, _r)   BOOST_PP_SEQ_ELEM(1, _seq)

//
/// \brief Macro used to get the enum value FALSE BRANCH
//
#define DECLARE_VALUE_OR_AUTOINC_FALSE_BRANCH(_seq, _r)  _r

//
/// \brief Macro used to get the enum value (either specified, or generated automatically)
///		((Off))      -> 0  (or another incremented value)
///		((Off)(5))   -> 5
/// ! the BOOST_PP_IF just return the macro name (true branch or false branch). It must not return directly the branches content
///   to preserve compatibility with gcc/clang
//
#define DECLARE_VALUE_OR_AUTOINC(_seq, _r) BOOST_PP_IF(DECLARE_VALUE_OR_AUTOINC_CONDITION(_seq), DECLARE_VALUE_OR_AUTOINC_TRUE_BRANCH, DECLARE_VALUE_OR_AUTOINC_FALSE_BRANCH)(_seq, _r)








//
/// \brief Macro used to get the enum name string CONDITION
//
#define DECLARE_NAME_OR_STRINGIZE_CONDITION(_seq) BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(_seq), 2)

//
/// \brief Macro used to get the enum name string TRUE BRANCH
//
#define DECLARE_NAME_OR_STRINGIZE_TRUE_BRANCH(_seq)   std::string( ENUM_EXTRACT_VALUE(_seq) )

//
/// \brief Macro used to get the enum name string FALSE BRANCH
//
#define DECLARE_NAME_OR_STRINGIZE_FALSE_BRANCH(_seq)  std::string(BOOST_PP_STRINGIZE( ENUM_EXTRACT_NAME_RAW(_seq)))

//
/// \brief Macro used to get the enum name string (either specified, or generated automatically)
///		((Off))              -> "off"
///		((Off)("powerOff"))  -> "powerOff"
/// ! the BOOST_PP_IF just return the macro name (true branch or false branch). It must not return directly the branches content
///   to preserve compatibility with gcc/clang
//
#define DECLARE_NAME_OR_STRINGIZE(_seq) BOOST_PP_IF(DECLARE_NAME_OR_STRINGIZE_CONDITION(_seq), DECLARE_NAME_OR_STRINGIZE_TRUE_BRANCH, DECLARE_NAME_OR_STRINGIZE_FALSE_BRANCH)(_seq)


//
/// \brief Macro used to declare one enum value
//
#define DECLARE_ENUM_STATIC_VALUE(r, _enumName, elem)   static const ENUM_CLASSNAME(_enumName) ENUM_EXTRACT_NAME(elem);								

//
/// \brief Macro used to declare all the enum values
//
#define DECLARE_ENUM_STATIC_VALUES(_enumName, _seq)	BOOST_PP_SEQ_FOR_EACH(DECLARE_ENUM_STATIC_VALUE, _enumName, _seq)     


//
/// \brief Macro used to declare one enum value (constant value, usable in switch/case statements)
//
#define DECLARE_DOMAIN_VALUE(r, _notused, i, elem)   ENUM_EXTRACT_DOMAINNAME(elem)  = DECLARE_VALUE_OR_AUTOINC(elem, i),								

//
/// \brief Macro used to declare all the enum values (constant value, usable in switch/case statements)
//
#define DECLARE_DOMAIN_VALUES(_seq)	                              \
      enum domain   {                                             \
         BOOST_PP_SEQ_FOR_EACH_I(DECLARE_DOMAIN_VALUE, _, _seq)   \
            };



//
/// \brief Macro used to declare in header the string of one value
//
#define ENUM_DECLARE_STATIC_CONST_NAME(r, _enumName, elem)	static const std::string ENUM_EXTRACT_CONST_STRINGNAME(elem);

//
/// \brief Macro used to declare in header the string of each value
//
#define ENUM_DECLARE_STATIC_CONST_NAMES(_seq)   BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME, _, _seq)     



//
/// \brief Macro used to declare the Enum class header with possibility of export/import from/to a shared library
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _export       The export class specifier (can be __declspec(dllexport/dllimport) for MSVC, or extern "C" for unix systems
/// \param [in] _seq          The enumeration sequence
//
#define DECLARE_ENUM_HEADER_SHARED_TYPE(_enumName, _export, _type, _seq) 																	\
   class _export ENUM_CLASSNAME(_enumName): public shared::enumeration::IExtendedEnum														\
   	{																																		\
	public:																																	\
		DECLARE_DOMAIN_VALUES(_seq)						    																				\
		DECLARE_ENUM_STATIC_VALUES(_enumName, _seq)						    																\
		ENUM_CLASSNAME(_enumName)();																										\
		ENUM_CLASSNAME(_enumName)(const ENUM_CLASSNAME(_enumName) & valueTocopy);															\
		explicit ENUM_CLASSNAME(_enumName)(const std::string & valueAsString);	    																\
		explicit ENUM_CLASSNAME(_enumName)(const char * valueAsString);	    																		\
		explicit ENUM_CLASSNAME(_enumName)(const _type valueAsInt);	    																			\
		virtual ~ENUM_CLASSNAME(_enumName)();																								\
		operator _type() const;																												\
		operator std::string() const;																										\
		ENUM_CLASSNAME(_enumName) & operator=(_type const& obj);																			\
		ENUM_CLASSNAME(_enumName) & operator=(std::string const& obj);																		\
		ENUM_CLASSNAME(_enumName) & operator=(const char * obj);																			\
		ENUM_CLASSNAME(_enumName) & operator=(const ENUM_CLASSNAME(_enumName) & obj);														\
		ENUM_CLASSNAME(_enumName) const operator() () const;																				\
		const std::string & toString() const;																								\
      int toInteger() const;																										\
		virtual void fromString(const std::string & val);																					\
		virtual const std::multimap<int, std::string> getAllValuesAndStrings() const;														\
		virtual const std::vector<int> getAllValues() const;																				\
		virtual const std::vector<std::string> getAllStrings() const;																		\
		virtual const std::string & getName() const;																						\
		static ENUM_CLASSNAME(_enumName) parse(const std::string & val);																	\
		static bool isDefined(const std::string & stringValue);																				\
		static bool isDefined(const int intValue);																							\
      static std::string toAllString(const std::string & separator); \
	private:																									                            \
		static std::string m_name;																											\
		_type   m_value;																					                                \
		ENUM_DECLARE_STATIC_CONST_NAMES(_seq);																								\
   	};																											

//
/// \brief Macro used to declare the Enum class header 
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enumeration sequence ((On)(0)) ((Off)(1)) ((Dim)(2))
//
#define DECLARE_ENUM_HEADER(_enumName, _seq)	DECLARE_ENUM_HEADER_SHARED_TYPE(_enumName, , int, _seq)			

//
/// \brief Macro used to declare the Enum class header with an underlying type
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enumeration sequence ((On)(0)) ((Off)(1)) ((Dim)(2))
/// \param [in] _type         The enumeration type (int, unsigned int, short,....)
//
#define DECLARE_ENUM_HEADER_TYPE(_enumName, _seq, _type)	DECLARE_ENUM_HEADER_SHARED_TYPE(_enumName, , _type, _seq)			

//
/// \brief Macro used to declare the Enum class header with possibility of export/import from/to a shared library
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _export       The export class specifier (can be __declspec(dllexport/dllimport) for MSVC, or extern "C" for unix systems
/// \param [in] _seq          The enumeration sequence
//
#define DECLARE_ENUM_HEADER_SHARED(_enumName, _export, _seq)   DECLARE_ENUM_HEADER_SHARED_TYPE(_enumName, _export, int, _seq)			




//
/// \brief Macro used to declare the implementation of GetAsString method for one case
//
#define ENUM_DECLARE_GETASSTRING(r, _enumName, elem) case ENUM_EXTRACT_DOMAINNAME(elem): return ENUM_EXTRACT_CONST_STRINGNAME(elem);


//
/// \brief Macro used to declare the implementation of GetAsString method
//
#define ENUM_DECLARE_GETASSTRING_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_GETASSTRING, _, _seq)     



//
/// \brief Macro used to declare the implementation of SetFromString method for one case
//
#define ENUM_DECLARE_SETFROMSTRING(r, _enumName, elem)									                 \
   if (boost::iequals(val, ENUM_EXTRACT_NAME_IMPL( ENUM_EXTRACT_NAME_RAW(elem) ).toString()))            \
      m_value = ENUM_EXTRACT_DOMAINNAME(elem);                                                           \
      else                                                                                


//
/// \brief Macro used to declare the implementation of SetFromString method
//
#define ENUM_DECLARE_SETFROMSTRING_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_SETFROMSTRING, _, _seq)     




//
/// \brief Macro used to declare the implementation of adding one enumeration value to a (multi)map
//
#define ENUM_DECLARE_ADD_ONE_VALUE_AND_STRING(r, _var, elem)  _var.insert(std::make_pair((int)ENUM_EXTRACT_DOMAINNAME(elem), ENUM_EXTRACT_CONST_STRINGNAME(elem)));



//
/// \brief Macro used to declare the implementation of values lisiting method
//
#define ENUM_DECLARE_LIST_ALL_VALUES_AND_STRINGS_IMPL(_var, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ADD_ONE_VALUE_AND_STRING, _var, _seq)     



//
/// \brief Macro used to declare the implementation of adding one enumeration value to a (multi)map
//
#define ENUM_DECLARE_ADD_ONE_VALUE(r, _var, elem)  _var.push_back((int)ENUM_EXTRACT_DOMAINNAME(elem));



//
/// \brief Macro used to declare the implementation of values lisiting method
//
#define ENUM_DECLARE_LIST_ALL_VALUES_IMPL(_var, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ADD_ONE_VALUE, _var, _seq)     


//
/// \brief Macro used to declare the implementation of adding one enumeration value to a (multi)map
//
#define ENUM_DECLARE_ADD_ONE_STRING(r, _var, elem)  _var.push_back(ENUM_EXTRACT_CONST_STRINGNAME(elem));



//
/// \brief Macro used to declare the implementation of values lisiting method
//
#define ENUM_DECLARE_LIST_ALL_STRINGS_IMPL(_var, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ADD_ONE_STRING, _var, _seq)     

//
/// \brief Macro used to declare the implementation of adding one enumeration value to a (multi)map
//
#define ENUM_DECLARE_ONE_TO_ALL_STRINGS_IMPL(r, _var, elem)  result += ENUM_EXTRACT_CONST_STRINGNAME(elem) + _var;


//
/// \brief Macro used to declare the implementation of values lisiting method
//
#define ENUM_DECLARE_LIST_TO_ALL_STRINGS_IMPL(_var, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ONE_TO_ALL_STRINGS_IMPL, _var, _seq)     




//
/// \brief Macro used to declare the implementation of exists(int) method for one case
//
#define ENUM_DECLARE_ISDEFINED_INT(r, _enumName, elem)  case ENUM_EXTRACT_DOMAINNAME(elem):



//
/// \brief Macro used to declare the implementation of exists(int) method
//
#define ENUM_DECLARE_ISDEFINED_INT_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ISDEFINED_INT, _, _seq)     





//
/// \brief Macro used to declare the implementation of exists(string) method for one case
//
#define ENUM_DECLARE_ISDEFINED_STRING(r, _enumName, elem)	                                             \
   if (boost::iequals(val, ENUM_EXTRACT_NAME_IMPL(ENUM_EXTRACT_NAME_RAW(elem)).toString()))              \
      return true;                                                                                       \
      else                                                                                


//
/// \brief Macro used to declare the implementation of exists(string) method
//
#define ENUM_DECLARE_ISDEFINED_STRING_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_ISDEFINED_STRING, _, _seq)     


//
/// \brief Macro used to declare the definition of one static enum name string with custom definition
//   
//#define ENUM_DECLARE_STATIC_CONST_ENUM_IMPL(r, _fullEnumNameAndEnumName, elem) const BOOST_PP_SEQ_ELEM(1, _fullEnumNameAndEnumName) BOOST_PP_SEQ_ELEM(0, _fullEnumNameAndEnumName)::ENUM_EXTRACT_CONST_NAME( elem )( ENUM_EXTRACT_VALUE(elem) );
#define ENUM_DECLARE_STATIC_CONST_ENUM_IMPL(r, _fullEnumNameAndEnumName, i, elem) \
   const BOOST_PP_SEQ_ELEM(0, _fullEnumNameAndEnumName) BOOST_PP_SEQ_ELEM(0, _fullEnumNameAndEnumName)::ENUM_EXTRACT_CONST_NAME( elem )( BOOST_PP_SEQ_ELEM(0, _fullEnumNameAndEnumName)::ENUM_EXTRACT_DOMAINNAME(elem) ); 

//
/// \brief Macro used to declare the definition of static enum name strings with custom definition
//   
#define ENUM_DECLARE_STATIC_CONST_ENUMS_IMPL(_fullEnumNameAndEnumName, _seq) BOOST_PP_SEQ_FOR_EACH_I(ENUM_DECLARE_STATIC_CONST_ENUM_IMPL, _fullEnumNameAndEnumName, _seq)     


//
/// \brief Macro used to declare the definition of one static enum name string
//   
#define ENUM_DECLARE_STATIC_CONST_NAME_IMPL(r, _fullEnumName, elem) const std::string _fullEnumName::ENUM_EXTRACT_CONST_STRINGNAME(elem) = DECLARE_NAME_OR_STRINGIZE(elem);

//
/// \brief Macro used to declare the definition of static enum name strings
//   
#define ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_fullClassifiedEnumName, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME_IMPL, _fullClassifiedEnumName, _seq)     


#define CHECK_VALUE(val)      if(!isDefined(val)) throw shared::exception::COutOfRange("Invalid enum value"); 

//
/// \brief Macro used to declare the Enum class implementation with possibility of defining it as a nested class
/// \param [in] _fullClassifiedEnumName The enumeration name if nested : ex : CMyClass::EMyEnum
/// \param [in] __enumName       	The namespace to use without full qualified name : EMyEnum
/// \param [in] _type          The enumeration inner type (can be int, short,...)
/// \param [in] _seq          The enumeration sequence ((Off)("off")) or ((Off)) . The second parameter is optional. By default this is the first parameter stringized as lower
//
#define DECLARE_ENUM_IMPLEMENTATION_NESTED_TYPE(_fullClassifiedEnumName, _enumName, _type, _seq)                                                \
   std::string _fullClassifiedEnumName::m_name = std::string(BOOST_PP_STRINGIZE(_enumName));													\
   ENUM_DECLARE_STATIC_CONST_ENUMS_IMPL((_fullClassifiedEnumName)(_enumName), _seq)                                                             \
   ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_fullClassifiedEnumName, _seq)                                                                          \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)() : m_value( ENUM_EXTRACT_NAME(BOOST_PP_SEQ_HEAD(_seq)) ) {CHECK_VALUE(m_value); }        \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const _type value) : m_value(value) {CHECK_VALUE(m_value);}                               \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const std::string & val): m_value(parse(val).m_value) {}                                  \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const char * val): m_value(parse(std::string(val)).m_value) {}                            \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const ENUM_CLASSNAME(_enumName) & val): m_value(val.m_value) {CHECK_VALUE(m_value);}      \
   _fullClassifiedEnumName::~ENUM_CLASSNAME(_enumName)() {}                                                                                     \
   _fullClassifiedEnumName::operator _type() const { return m_value; }                                                                          \
   int _fullClassifiedEnumName::toInteger() const { return m_value; }                                                                     \
   _fullClassifiedEnumName::operator std::string() const { return toString(); }                                                                 \
   _fullClassifiedEnumName const _fullClassifiedEnumName::operator() () const { return _fullClassifiedEnumName(m_value); }				            \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(_type const& obj)                                                               \
      {                                                                                                                                         \
      m_value = obj;                                                                                                                            \
      CHECK_VALUE(m_value);                                                                                                                     \
      return *this;                                                                                                                             \
      }                                                                                                                                         \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(const ENUM_CLASSNAME(_enumName) & obj)                                          \
      {                                                                                                                                         \
      m_value = obj.m_value;                                                                                                                    \
      CHECK_VALUE(m_value);                                                                                                                     \
      return *this;                                                                                                                             \
      }                                                                                                                                         \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(std::string const& obj)                                                         \
      {                                                                                                                                         \
      m_value = parse(obj).m_value;                                                                                                             \
      return *this;                                                                                                                             \
      }                                                                                                                                         \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(const char * obj)                                                               \
      {                                                                                                                                         \
      m_value = parse(std::string(obj)).m_value;                                                                                                \
      return *this;                                                                                                                             \
      }                                                                                                                                         \
   const std::string & _fullClassifiedEnumName::toString() const                                                                                \
   	{                                                                                                                                           \
      switch(m_value)                                                                                                                           \
            {                                                                                                                                   \
		   ENUM_DECLARE_GETASSTRING_IMPL(_seq)                                                                                                  \
		   default : throw shared::exception::COutOfRange("Invalid enum value");                                                                \
            }                                                                                                                                   \
      }                                                                                                                                         \
	void _fullClassifiedEnumName::fromString(const std::string & val)                                                                           \
   	{                                                                                                                                           \
		ENUM_DECLARE_SETFROMSTRING_IMPL(_seq)                                                                                                   \
      throw shared::exception::COutOfRange(val);                                                                                                \
   	}                                                                                                                                           \
   _fullClassifiedEnumName _fullClassifiedEnumName::parse(const std::string & val)                                                              \
      {                                                                                                                                         \
      ENUM_CLASSNAME(_enumName) local;                                                                                                          \
      local.fromString(val);                                                                                                                    \
      return local;                                                                                                                             \
      }                                                                                                                                         \
   bool _fullClassifiedEnumName::isDefined(const std::string & val)                                                                             \
      {                                                                                                                                         \
      ENUM_DECLARE_ISDEFINED_STRING_IMPL(_seq)                                                                                                  \
      return false;                                                                                                                             \
      }                                                                                                                                         \
   bool _fullClassifiedEnumName::isDefined(const _type val)                                                                                     \
      {                                                                                                                                         \
      switch(val)                                                                                                                               \
            {                                                                                                                                   \
         ENUM_DECLARE_ISDEFINED_INT_IMPL(_seq)                                                                                                  \
            return true;                                                                                                                        \
         default :                                                                                                                              \
            return false;                                                                                                                       \
            }                                                                                                                                   \
      }																																			\
	const std::multimap<int, std::string> _fullClassifiedEnumName::getAllValuesAndStrings() const												\
	{																																			\
		std::multimap<int, std::string> allValues;																								\
        ENUM_DECLARE_LIST_ALL_VALUES_AND_STRINGS_IMPL(allValues, _seq)                                                                          \
		return allValues;																														\
	}																																			\
	const std::vector<int> _fullClassifiedEnumName::getAllValues() const																		\
	{																																			\
		std::vector<int> allValues;																												\
        ENUM_DECLARE_LIST_ALL_VALUES_IMPL(allValues, _seq)                                                                                      \
		return allValues;																														\
	}																																			\
	const std::vector<std::string> _fullClassifiedEnumName::getAllStrings() const																\
	{																																			\
		std::vector<std::string> allStrings;																									\
        ENUM_DECLARE_LIST_ALL_STRINGS_IMPL(allStrings, _seq)                                                                                    \
		return allStrings;																														\
	}																																			\
   std::string _fullClassifiedEnumName::toAllString(const std::string & separator)                            \
   {                                                                                                        \
      std::string result;                                                                                   \
      ENUM_DECLARE_LIST_TO_ALL_STRINGS_IMPL(separator, _seq)                                                \
      if(boost::algorithm::ends_with(result, separator)) result.erase(result.rfind(separator));             \
      return result;                                                                                        \
   }                                                                                                        \
	const std::string & _fullClassifiedEnumName::getName() const															\
	{																																			\
		return m_name;																															\
	}


//
/// \brief Macro used to declare the Enum class implementation
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enuemration sequence (Off)(On)(Dim)
//   
#define DECLARE_ENUM_IMPLEMENTATION(_enumName, _seq)  DECLARE_ENUM_IMPLEMENTATION_NESTED_TYPE(_enumName, _enumName, int, _seq)

//
/// \brief Macro used to declare a NESTED Enum class implementation
/// \param [in] _fullClassifiedEnumName     The full qualified name: CMyClass::ETest
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enuemration sequence (Off)(On)(Dim)
//  
#define DECLARE_ENUM_IMPLEMENTATION_NESTED(_fullClassifiedEnumName, _enumName, _seq) DECLARE_ENUM_IMPLEMENTATION_NESTED_TYPE(_fullClassifiedEnumName, _enumName, int, _seq)

//
/// \brief Macro used to declare the Enum class implementation
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _type         The real type of enumeration values (int, short,...)
/// \param [in] _seq          The enuemration sequence (Off)(On)(Dim)
//   
#define DECLARE_ENUM_IMPLEMENTATION_TYPE(_enumName, _type, _seq)  DECLARE_ENUM_IMPLEMENTATION_NESTED_TYPE(_enumName, _enumName, _type, _seq)

