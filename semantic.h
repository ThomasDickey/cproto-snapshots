/* $Id: semantic.h,v 4.1 1994/10/12 14:12:48 cthuang Exp $
 *
 * Declarations of semantic action routines
 */

extern void new_decl_spec ARGS((
    DeclSpec *decl_spec, char *text, long offset, int flags));
extern void free_decl_spec ARGS((
    DeclSpec *decl_spec));
extern void join_decl_specs ARGS((
    DeclSpec *result, DeclSpec *a, DeclSpec *b));
extern void check_untagged ARGS((
    DeclSpec *decl_spec));
extern Declarator *new_declarator ARGS((
    char *text, char *name, long offset));
extern void free_declarator ARGS((
    Declarator *d));
extern void new_decl_list ARGS((
    DeclaratorList *decl_list, Declarator *declarator));
extern void free_decl_list ARGS((
    DeclaratorList *decl_list));
extern void add_decl_list ARGS((
    DeclaratorList *to, DeclaratorList *from, Declarator *declarator));
extern Parameter *new_parameter ARGS((
    DeclSpec *decl_spec, Declarator *declarator));
extern void free_parameter ARGS((
    Parameter *param));
extern void new_param_list ARGS((
    ParameterList *param_list, Parameter *param));
extern void free_param_list ARGS((
    ParameterList *param_list));
extern void add_param_list ARGS((
    ParameterList *to, ParameterList *from, Parameter *param));
extern void new_ident_list ARGS((
    ParameterList *param_list));
extern void add_ident_list ARGS((
    ParameterList *to, ParameterList *from, char *name));
extern void set_param_types ARGS((
    ParameterList *params, DeclSpec *decl_spec, DeclaratorList *declarators));
extern void gen_declarations ARGS((
    DeclSpec *decl_spec, DeclaratorList *decl_list));
extern void gen_prototype ARGS((
    DeclSpec *decl_spec, Declarator *declarator));
extern void gen_func_declarator ARGS((
    Declarator *declarator));
extern void gen_func_definition ARGS((
    DeclSpec *decl_spec, Declarator *declarator));
