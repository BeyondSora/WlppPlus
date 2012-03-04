/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 *
 * This namespace is used to handle errors.
 * Include this header in every other class/namespace that might error out,
 *  preferablly the last one to avoid any kind of dependency issues.
 ******************************************************************************/

#ifndef ERROR_H
#define ERROR_H

#include <iostream>

#include "common.h"

namespace error {

// ErrorCode enumerator,
//  contains error codes for known error conditions.
// Do NOT initialize an instance of this enumerator,
//  since it has no use outside of ErrorObject.
enum ErrorCode {
    // Default retCode == 1

    // File namespace error codes.
    FILE_CANNOT_OPEN,       // retCode == 4
    FILE_CANNOT_CLOSE,      // retCode == 5
};

// Interface for ErrorObject.
class ErrorObjectInterface {
    public:
        virtual ~ErrorObjectInterface();
        virtual std::string toString() const;
        int retCode() const;
    protected:
        ErrorCode errCode_;
        std::string errLocation_;    // Where did this error occur?
        std::string errMsg_;

        ErrorObjectInterface(ErrorCode errCode,
                             std::string errLocation,
                             std::string errMsg);
};

// Smart pointer container for ErrorObjectInterface*.
class ErrorObjectPtr {
    public:
        ErrorObjectPtr(ErrorObjectInterface *err);
        ~ErrorObjectPtr();
        ErrorObjectInterface* operator->();
    private:
        ErrorObjectInterface *err_;
};

}

#endif//ERROR_H
