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

    // File specific error codes.
    FILE_CANNOT_OPEN,       // retCode == 4
    FILE_CANNOT_CLOSE,      // retCode == 5

    // Scanning specific error codes.
    LEX_ERROR,              // retCode = 8

    // Production rule related error codes.
    NO_MATCHING_PROD_RULE,  // retCode == 11

    // Parse Tree error codes.
    TREE_IS_NULL,           // retCode == 16

    // Semantic Analysis error codes.
    EXISTS_DUPLICATE_SYMBOL,// retCode == 21
    EXPR_FAULT,             // retCode == 22
    TERM_FAULT,             // retCode == 23
    FTOR_FAULT,             // retCode == 24
    LVAL_FAULT,             // retCode == 25
    RET_NO_MATCH,           // retCode == 26
    LHS_RHS_NOT_SAME,       // retCode == 27
    COMPARISON_FAULT,       // retCode == 28
};

// Interface for ErrorObject.
// Always use ErrorObjectPtr to wrap around it.
// If any additional features not provided here are needed,
//  inherit this class as parent class of ErrorObject in
//  the corresponding namespace where the
//  additional features are intended to be used.
class ErrorObjectInterface {
    friend class ErrorObjectPtr;
    public:
        virtual std::string toString() const;
        int retCode() const;
    protected:
        ErrorCode errCode_;
        std::string errLocation_;    // Where did this error occur?
        std::string errMsg_;

        ErrorObjectInterface(ErrorCode errCode,
                             std::string errLocation,
                             std::string errMsg);
        virtual ~ErrorObjectInterface();
};

// Basic ErrorObject with no additional features.
// Use this when there is no need for additional error handling.
// Do NOT inherit this class!
class ErrorObject: public ErrorObjectInterface {
    public:
        ErrorObject(ErrorCode errCode,
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

void errOut(ErrorObjectInterface const* err);

}

#endif//ERROR_H
