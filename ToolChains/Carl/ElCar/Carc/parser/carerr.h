//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CARERR_H__
#define __CARERR_H__

typedef enum CARErrorNo {
    CAR_NoError = 0,

    // Warnings
    CAR_WarningStart        = 0x0001,
    CAR_W_LocalResult,
    CAR_W_LocalArg,
    CAR_W_NoMethods,
    CAR_W_LocalParent,
    CAR_W_NoClassInterfaces,
    CAR_W_LoadLibrary,
    CAR_W_TooManyLibraries,
    CAR_W_LocalClass,
    CAR_W_DupMethodName,
    CAR_W_UnexpectFileType,
    CAR_W_NoAutoParamAttrib,
    CAR_W_IllegalMemberName,
    CAR_W_IllegalCharacterInURL,
    CAR_W_NoArrayBufSize,
    CAR_W_IgnoreThreadMode,
    CAR_W_DecrepitTypeUsage,
    CAR_W_NoProperties,
    CAR_W_CARUunmUndef,

    // Errors
    CAR_ErrorStart          = 0x0100,
    CAR_E_UnexpectEOF,
    CAR_E_UnexpectSymbol,
    CAR_E_UnexpectChar,
    CAR_E_SymbolTooLong,
    CAR_E_IllegalChar,
    CAR_E_LibraryProject,
    CAR_E_UunmDifferFromCarName,
    CAR_E_LoadLibrary,
    CAR_E_ExpectSymbol,
    CAR_E_UuidFormat,
    CAR_E_OutOfMemory,
    CAR_E_AttribConflict,
    CAR_E_MscomNoUuid,
    CAR_E_IllegalValue,
    CAR_E_DupEntry,
    CAR_E_FullEntry,
    CAR_E_NameConflict,
    CAR_E_NotFound,
    CAR_E_UndefinedSymbol,
    CAR_E_UuidNoMscom,
    CAR_E_RedefUuid,
    CAR_E_IllegalInterfaceProperties,
    CAR_E_IllegalClassProperties,
    CAR_E_ExpectInterfaceName,
    CAR_E_ExpectClassName,
    CAR_E_ExpectStructName,
    CAR_E_ExpectEnumName,
    CAR_E_UndefType,
    CAR_E_ExpectMethodName,
    CAR_E_ExpectParamName,
    CAR_E_TypeConflict,
    CAR_E_AspectUse,
    CAR_E_RegimeNoAspect,
    CAR_E_NotAspect,
    CAR_E_NotClass,
    CAR_E_TooManyParents,
    CAR_E_NestedType,
    CAR_E_AsyncOut,
    CAR_E_NoMethods,
    CAR_E_DupUuid,
    CAR_E_NoMainClass,
    CAR_E_ClassInterfaces,
    CAR_E_NoClassInterfaces,
    CAR_E_LoadSystemLib,
    CAR_E_NestedStruct,
    CAR_E_MergeCLS,
    CAR_E_GenDisp,
    CAR_E_DupMethodName,
    CAR_E_InheritNoVirtual,
    CAR_E_NoClasses,
    CAR_E_OpenFile,
    CAR_E_NestedInherit,
    CAR_E_OutParameterInCtor,
    CAR_E_ParameterInSingletonCtor,
    CAR_E_InvalidMemberName,
    CAR_E_IllegalClassName,
    CAR_E_IllegalAspectName,
    CAR_E_IllegalRegimeName,
    CAR_E_IllegalDomainName,
    CAR_E_IllegalGenericName,
    CAR_E_IllegalInterfaceName,
    CAR_E_IllegalCallbacksOrDelegatesName,
    CAR_E_IlleagalSizeType,
    CAR_E_ExpectConstName,
    CAR_E_ExpectGenericName,
    CAR_E_AggregateUse,
    CAR_E_DupNameWithModule,
    CAR_E_OutParameterWithCallback,
    CAR_E_ContructorWithAspect,
    CAR_E_LocalUse,
    CAR_E_IllegalVersion,
    CAR_E_NoConstructor,
    CAR_E_NoVirtualInterface,
    CAR_E_InheritanceConflict,
    CAR_E_CouldntHasCtor,
    CAR_E_CallbackInterface,
    CAR_E_LocalConflict,
    CAR_E_NoParameters,
    CAR_E_CouldNotInheritGeneric,
    CAR_E_NotGeneric,
    CAR_E_NoImplIntfOfGeneric,
    CAR_E_InterfaceShouldBeLocal,
    CAR_E_ParamNoAttrib,
    CAR_E_IllegalParamAttrib,
    CAR_E_IllegalParamType,
    CAR_E_CalleeParam,
    CAR_E_IllegalTypeUsage,
    CAR_E_GenericCouldntHaveVirtualInterface,
    CAR_E_NestedCARQuient,
    CAR_E_IllegalPrivilegedInterface,
    CAR_E_CouldNotInheritFinalClass,
    CAR_E_DeclaredInterfaceProp,
    CAR_E_DeclaredClassProp,
    CAR_E_IllegalFinalUsage,
	CAR_E_UnmatchedInterface,
	CAR_E_CalleeDecl,
	CAR_E_ExpectParamAttrib,
	CAR_E_InvalidUUNM,
	CAR_E_NotIntegerConst,
};

extern "C" void ErrorReport(CARErrorNo, ...);
extern void SuppressWarning();
extern void TreatWarningAsError();
extern void ErrorReporterInit(const char *);
extern void ErrorStringFix(const char *, const char *);
extern void DisableWarning(int);
extern void EnableWarning(int);
extern void DisableErrorCounting();
extern void EnableErrorCounting();
extern void CreateError(int, const char *, const char *);
extern void * SaveErrorContext();
extern void RestoreErrorContext(void *);

extern int g_nErrorNumber;
extern int g_nWarningNumber;

#endif // __CARERR_H__
