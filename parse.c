#include <stdio.h>
#include "cflat.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct Scope Scope;
struct Scope {
    struct { char *key; Type *val; } *vars; // symbol table
    Scope *next;
};

void declarator(Token **curr_tk, Type* ty, VarAttrs *attrs, bool is_func_params);
void direct_declarator(
    Token **curr_tk,
    Type *ty,
    VarAttrs *attrs,
    bool is_func_params,
    bool is_func_def_params
);

Scope *scope_stack_head;

// utils

inline Scope *new_scope()
{
    Scope *res = malloc(sizeof(Scope));
    res->vars = NULL;
    return res;
}

inline void next_tk(Token **curr_tk) {
    if ((*curr_tk)->next == NULL) {
        fprintf(stderr, "internal err: called next_tk when next is NULL\n");
        exit(1);
    }
}

inline TokenType tk_ty(Token **curr_tk)
{
    if (*curr_tk == NULL) {
        fprintf(stderr, "internal err: called tk_ty on NULL\n");
        exit(1);
    }

    return (*curr_tk)->ty;
}


// parsing

/* primary_expr = TK_ID
 *              | TK_NUM
 *              | TK_CHAR
 *              | TK_STR
 *              | "(" exprs ")"
 * LATER        | generic_selection
 *
 * NOTE: generic_selection for preprocesser I think
 * NOTE: we treat TK_STR like a VarRef (to a compiler-generated variable)
 */
Expr *primary_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    Expr *res = malloc(sizeof(Expr));
    switch (tk_ty(curr_tk)) {
        case TK_ID:
            break;
        case TK_STR:
            break;
        case TK_NUM: case TK_CHAR:
            break;
        case TK_LPAREN:
            break;
        default:
            break;
    }

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* postfix_expr = primary_expr
 *              | postfix_expr "[" expr "]"
 *              | postfix_expr "(" ( assnt_expr ("," assnt_expr)* )? ")"
 *              | postfix_expr ("." || "->") TK_ID
 *              | postfix_expr ("++" | "--")
 *              | "(" type_name ")" "{" init_list ","? "}"
 */
Expr *postfix_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* cast_expr = ("(" unary_expr ")")* unary_expr
 */
Expr *cast_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* unary_expr = postfix_expr
 *            | ("++" | "--") unary_expr
 *            | ("&" | "*" | "+" | "-" | "~" | "!") cast_expr
 *            | TK_SIZEOF unary_expr
 *            | (TK_SIZEOF | TK__ALIGNOF) "(" type_name ")"
 */
Expr *unary_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* mul_expr = cast_expr (("*" | "/" | "%") cast_expr)*
 */
Expr *mul_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* add_expr = mul_expr (("+" | "-") mul_expr)*
 */
Expr *add_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* shift_expr = add_expr (("<<" | ">>") add_expr)*
 */
Expr *shift_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* cond_expr = LOR_expr ("?" exprs ":" cond_expr)?
 */
Expr *cond_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* assnt_expr = cond_expr
 *            | unary_expr ("="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|=")
 *              assnt_expr
 */
Expr *assnt_expr(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* exprs = assnt_expr ("," assnt_expr)*
 */
Expr *exprs(Token **curr_tk, int *expr_cnt)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* decl_specs = ( TK_TYPEDEF | TK_EXTERN | TK_STATIC | TK__THR_LOC
 *               | TK_AUTO | TK_REGISTER | TK_VOID | TK_CHAR
 *               | TK_SHORT | TK_INT | TK_LONG | TK_FLOAT | TK_DOUBLE
 *               | TK_SIGNED | TK_UNSIGNED | TK__BOOL | TK__COMPLEX
 *               | TK_CONST | TK_RESTRICT | TK_VOLATILE | TK__Atomic
 *               | TK_INLINE | TK__NORETURN
 *               | atomic_type_spec
 *               | struct_or_union_spec
 *               | enum_spec
 *               | align_spec
 *               | TK_ID )+
 *
 * NOTE: at most one storage_class_spec is allowed
 *     (EXCEPT: TK__THR_LOC with TK_STATIC OR TK_EXTERN)
 *
 * NOTE: TK__THR_LOC can't be used in func decls OR defs
 * NOTE: TK_STATIC, TK__THR_LOC, TK_AUTO, TK_REGISTER
 *     all carry recursively to members of struct or union
 */
Type *decl_specs(Token **curr_tk, VarAttrs *attrs, bool is_func_decl)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* pointers = '*' type_qualifier* pointers?
 */
int pointers(Token **curr_tk, Type *ty, VarAttrs *attrs)
{
    int ptr_cnt;
    while (tk_ty(curr_tk) == TK_STAR) {
        next_tk(curr_tk);
        while (tk_ty(curr_tk) == TK_CONST
                || tk_ty(curr_tk) == TK_RESTRICT
                || tk_ty(curr_tk) == TK_VOLATILE
                || tk_ty(curr_tk) == TK__ATOMIC) {

            switch (tk_ty(curr_tk)) {
                case TK_STATIC:
                    attrs->is_static = true;
                    break;
                case TK_CONST:
                    break;
                case TK_RESTRICT:
                    break;
                case TK_VOLATILE:
                    break;
                case TK__ATOMIC:
                    break;
                default:
                    /* problem */
                    return 0;
            }

            next_tk(curr_tk);
        }
    }

    return ptr_cnt;
}

/* param_decl = decl_specs (declarator | abstract_declarator)
 */
Obj *param_decl(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    VarAttrs *attrs;
    Type *ty = decl_specs(curr_tk, attrs, false);
    Obj *var = malloc(sizeof(Obj));
    var->ty = ty;
    var->attrs = attrs;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* param_t_list = param_decl ("," param_decl)* (, "...")?
 */
void param_t_list(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* func_id = TK_ID
 *         | "(" func_id ")"
 */
char* func_id(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;
    return NULL;
}

/* func_params = "(" param_t_list? ")"
 *             | "(" id_list? ")"
 */
Obj *func_params(Token **curr_tk, int *param_cnt)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* func_declarator = pointers? func_id func_params
 */
Obj *func_declarator(Token **curr_tk, Type *ty, VarAttrs *attrs)
{
    Token *tmp_tk_ptr = *curr_tk;

    pointers(&tmp_tk_ptr, ty, attrs);

    char* id = func_id(&tmp_tk_ptr);
    if (id == NULL) { /* problem */ }

    int param_cnt;
    func_params(&tmp_tk_ptr, &param_cnt);

    // TODO: make func and fill in things

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* direct_declarator = TK_ID
 *                   | "(" declarator ")"
 *                   | direct_declarator "[" type_qualifier* assnt_expr? "]"
 *                   | direct_declarator "[" type_qualifier* "*" ]"
 *                   | direct_declarator "[" static type-qualifier* assnt_expr "]"
 *                   | direct_declarator "[" type_qualifier+ static assnt_expr "]"
 *
 * NOTE: this is not as straightforward as it looks
 * NOTE: type_qualifier OR static only in func params
 * NOTE: "[" type_qualifier* "* "]" ONLY IN THE PARAMS OF A FUNCTION DECLARATION (NOT DEFINITION)
 */
void direct_declarator(
    Token **curr_tk,
    Type *ty,
    VarAttrs *attrs,
    bool is_func_params,
    bool is_func_def_params
)
{
    Token *tmp_tk_ptr = *curr_tk;

    char* id;
    if (tk_ty(&tmp_tk_ptr) == TK_LPAREN) {
        next_tk(&tmp_tk_ptr);
        declarator(&tmp_tk_ptr, ty, attrs, is_func_params);
        if (tk_ty(&tmp_tk_ptr) != TK_RPAREN) { /* problem */ }
        next_tk(&tmp_tk_ptr);
    } else if (tk_ty(&tmp_tk_ptr) == TK_ID) {
        id = tmp_tk_ptr->text.str;
        next_tk(&tmp_tk_ptr);
    }

    if (tk_ty(curr_tk) != TK_LBRACKET) {
        return;
    }

    // grab all type qualifiers or static

    while (tk_ty(curr_tk) == TK_STATIC
            || tk_ty(curr_tk) == TK_CONST
            || tk_ty(curr_tk) == TK_RESTRICT
            || tk_ty(curr_tk) == TK_VOLATILE
            || tk_ty(curr_tk) == TK__ATOMIC) {
        if (!is_func_params) {
            /* problem */
        }

        switch (tk_ty(curr_tk)) {
            case TK_STATIC:
                attrs->is_static = true;
                break;
            case TK_CONST:
                ty->is_const = true;
                break;
            case TK_RESTRICT:
                break;
            case TK_VOLATILE:
                break;
            case TK__ATOMIC:
                break;
            default:
                /* problem */
                return;
        }

        next_tk(curr_tk);
    }

    if (tk_ty(curr_tk) == TK_STAR) {
        // "*"
        if (!is_func_params || (is_func_params && is_func_def_params)) {
            /* problem */
        }

        // stuff -- I don't know what a "*" here does to type info

        // -----

        next_tk(curr_tk);
    } else if (tk_ty(curr_tk) == TK_RBRACKET) {
        // TODO: return
    } else {
        // assnt_expr
        // TODO
        assnt_expr(curr_tk);
        // ...

        if (tk_ty(curr_tk) != TK_RBRACKET) { /* problem */ }
        next_tk(curr_tk);
    }

    *curr_tk = tmp_tk_ptr;
    return;
}

/* declarator = pointers? direct_declarator
 */
void declarator(Token **curr_tk, Type* ty, VarAttrs *attrs, bool is_func_params)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* abstract_declarator = pointers? direct_abstract_declarator
 */
void abstract_declarator(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* direct_abstract_declarator = ( abstract_declarator )
 *                            | direct_abstract_declarator? "[" type_qualifier* assnt_expr? "]"
 *                            | direct_abstract_declarator? "[" "*" "]"
 *                            | direct_abstract_declarator? "[" static type-qualifier* assnt_expr "]"
 *                            | direct_abstract_declarator? "[" type_qualifier+ static assnt_expr "]"
 */
void direct_abstract_declarator(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* designation = ("[" const_expr "]" | "." TK_ID)+ "="
 */
void designation(Token **curr_tk)
{
    while (tk_ty(curr_tk) == TK_LBRACE || tk_ty(curr_tk) == TK_DOT) {
        if (tk_ty(curr_tk) == TK_LBRACE) {
            cond_expr(curr_tk);
        } else {
        }
    }
}

/* init_list = designation? init ("," designation? init)*
 */
void init_list(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* init = assnt_expr
 *      | "{" init_list (","?) "}"
 */
void init(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return;
}

/* init_declarator = declarator (= init)?
 *
 */
Obj *init_declarator(Token **curr_tk, Type *ty, VarAttrs *attrs, bool is_func_param)
{
    Token *tmp_tk_ptr = *curr_tk;

    declarator(&tmp_tk_ptr, ty, attrs, false);

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* init_declarator_list = init_declarator ("," init_declarator)*
 */
Obj *init_declarator_list(Token **curr_tk, int *var_cnt, Type *ty, VarAttrs *attrs)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* var_decl = decl_specs init_declarator_list? ;
 *      | static_assert_decl
 */
Obj *var_decl(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    VarAttrs *attrs;
    Type *ty = decl_specs(&tmp_tk_ptr, attrs, false);
    if (!ty) { /* problem: expected declaration specifiers */ }

    int var_cnt;
    init_declarator_list(&tmp_tk_ptr, &var_cnt, ty, attrs);

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* stmt = labeled_stmt
 *      | compound_stmt
 *      | expr_stmt
 *      | select_stmt
 *      | iter_stmt
 *      | jmp_stmt
 */
BlockItem *stmt(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* compound_stmt = "{" (var_decl | stmt)* "}"
 */
BlockItem *compound_stmt(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* func_def = decl_specs? func_declarator var_decl* compound_stmt
 *
 * NOTE: the decl* is legacy--no idea what to do with it
 * NOTE: extra care and checks needed--grammar not sufficient to parse
 */
Obj *func_def(Token **curr_tk)
{
    Token *tmp_tk_ptr = *curr_tk;

    VarAttrs *attrs;
    Type *ret_ty = decl_specs(&tmp_tk_ptr, attrs, true);
    if (!ret_ty) {
        // TODO: don't make new Type objects for funcs with no explicit ret type
        ret_ty = malloc(sizeof(Type));
        ret_ty->kind = TY_INT;
    }

    Type *func_ty = malloc(sizeof(Type));
    func_ty->kind = TY_FUNC;
    func_ty->ret_ty = ret_ty;

    Obj *func = func_declarator(&tmp_tk_ptr, func_ty, attrs);

    // after this point it's a func definition and not a declaration

    while (var_decl(&tmp_tk_ptr)); // consume but ignore (unsupported)

    BlockItem *block_items = compound_stmt(&tmp_tk_ptr);

    *curr_tk = tmp_tk_ptr;
    return NULL;
}

/* program = (func_def | func_decl | var_decl)*
 */
void parse(Token **curr_tk)
{
    while ((*curr_tk)->ty != TK_EOF) {
        Obj *func = func_def(curr_tk);
        if (func) continue;

        Obj *var = var_decl(curr_tk);
        if (!var) { /* err */ }
    }
}
