#pragma once

namespace arx
{

enum arxml_tag
{
    // BSWMD
    kModuleDef =            0,
    kParamContainerDef,
    kChoiceContainerDef,
    kEnumParamDef,
    kEnumLiteralDef,
    kIntParamDef,
    kFloatParamDef,
    kBoolParamDef,
    kReferenceDef,
    kChoiceReferenceDef,
    kFunctionNameDef,
    kSymbolicNameReferenceDef,
    kStringDef,


    // ECUC
    kModuleCfg,
    kParamContainerVal,
    kChoiceContainerVal,
    kNumParamVal,
    kTextParamVal,
    kReferenceVal,
    kChoiceReferenceVal
};

}