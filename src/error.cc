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
    }
}

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

}
