#ifndef CKX_AST_NODE_HPP
#define CKX_AST_NODE_HPP

namespace ckx
{

class ckx_ast_node
{
public:
    ckx_ast_node() = default;
    virtual ~ckx_ast_node() = 0;
};

class ckx_ast_translation_unit : public ckx_ast_node
{
public:
    ~ckx_ast_translation_unit() override final;
};

} // namespace ckx

#endif // CKX_AST_NODE_HPP
