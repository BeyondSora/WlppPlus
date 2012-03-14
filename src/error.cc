/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "error.h"

namespace error {

//
// ErrorObjectInterface class

ErrorObjectInterface::ErrorObjectInterface(ErrorCode errCode,
                                           std::string errLocation,
                                           std::string errMsg)
    : errCode_(errCode), errLocation_(errLocation), errMsg_(errMsg) {}

ErrorObjectInterface::~ErrorObjectInterface() {}

std::string ErrorObjectInterface::toString() const
{
    return "ERROR: " + errLocation_ + " - " + errMsg_;
}

int ErrorObjectInterface::retCode() const
{
    switch (errCode_) {
        default: return 1;

        case FILE_CANNOT_OPEN: return 4;
        case FILE_CANNOT_CLOSE: return 5;
        case LEX_ERROR: return 8;
        case NO_MATCHING_PROD_RULE: return 11;
        case TREE_IS_NULL: return 16;
        case EXISTS_DUPLICATE_SYMBOL: return 21;
        case EXPR_FAULT: return 22;
        case TERM_FAULT: return 23;
        case FTOR_FAULT: return 24;
        case LVAL_FAULT: return 25;
        case RET_NO_MATCH: return 26;
        case LHS_RHS_NOT_SAME: return 27;
        case COMPARISON_FAULT: return 28;
    }
}

///

//
// ErrorObject class

ErrorObject::ErrorObject(ErrorCode errCode,
                         std::string errLocation,
                         std::string errMsg)
    : ErrorObjectInterface(errCode, errLocation, errMsg) {}

///

//
// ErrorObjectPtr class

ErrorObjectPtr::ErrorObjectPtr(ErrorObjectInterface *err): err_(err) {}

ErrorObjectPtr::~ErrorObjectPtr()
{
    delete err_;
}

ErrorObjectInterface* ErrorObjectPtr::operator->()
{
    return err_;
}

///

void errOut(ErrorObjectInterface const* err)
{
    //out::msg(err->toString());
}

}
