#include "minishell.h"

const char *token_type_str(t_type_list type)
{
    if (type == comnd) return "COMMAND";
    if (type == PIPE) return "PIPE";
    if (type == INredirection) return "REDIRECT_IN";
    if (type == OUTredirection) return "REDIRECT_OUT";
    if (type == OUTappend) return "APPEND";
    if (type == HEREdocument) return "HEREDOC";
    return "UNKNOWN";
}

void print_tokens(t_tokenlist *token_list, const char *label, const char *color, const char *prefix)
{
    if (!token_list)
        return;

    printf("%s    %s%s: ", prefix, color, label);
    while (token_list)
    {
        printf("[%s] ", token_list->content);
        token_list = token_list->next;
    }
    printf(RESET "\n");
}

void draw_ast_vertical(t_tree_list *node, const char *prefix, int is_left)
{
    if (!node)
        return;

    // ├── or └──
    printf("%s%s── ", prefix, is_left ? "├" : "└");

    // Choose color
    const char *color;
    if (node->type == comnd)
        color = GREEN;
    else if (node->type == INredirection || node->type == OUTredirection ||
             node->type == OUTappend || node->type == HEREdocument)
        color = BLUE;
    else if (node->type == PIPE)
        color = YELLOW;
    else
        color = CYAN;

    // Print node type
    printf("%s%s%s\n", color, token_type_str(node->type), RESET);

    // Print COMMAND details
    if (node->type == comnd && node->cmd)
    {
        // Print arguments
        if (node->cmd->args)
            print_tokens(node->cmd->args, "ARGS", GREEN, prefix);

        // Print redirections (in and out)
        if (node->cmd->in)
            print_tokens(node->cmd->in, "REDIR_IN", BLUE, prefix);
        if (node->cmd->out)
            print_tokens(node->cmd->out, "REDIR_OUT", BLUE, prefix);
    }

    // Prepare new prefix for children
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    // Recurse: left first (conventionally), then right
    if (node->left || node->right)
    {
        if (node->left)
            draw_ast_vertical(node->left, new_prefix, 1);
        if (node->right)
            draw_ast_vertical(node->right, new_prefix, 0);
    }
}

// Root node is special: no prefix, just print it
void print_ast_topdown(t_tree_list *root)
{
    if (!root)
        return;

    // Root line
    const char *color;
    if (root->type == comnd)
        color = GREEN;
    else if (root->type == INredirection || root->type == OUTredirection ||
             root->type == OUTappend || root->type == HEREdocument)
        color = BLUE;
    else if (root->type == PIPE)
        color = YELLOW;
    else
        color = CYAN;

    printf("%s%s%s\n", color, token_type_str(root->type), RESET);

    // Print root CMD details if any
    if (root->type == comnd && root->cmd)
    {
        if (root->cmd->args)
            print_tokens(root->cmd->args, "ARGS", GREEN, "");
        if (root->cmd->in)
            print_tokens(root->cmd->in, "REDIR_IN", BLUE, "");
        if (root->cmd->out)
            print_tokens(root->cmd->out, "REDIR_OUT", BLUE, "");
    }

    // Then recurse
    if (root->left)
        draw_ast_vertical(root->left, "", 1);
    if (root->right)
        draw_ast_vertical(root->right, "", 0);
}