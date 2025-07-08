#include "../minishell.h"

int execute(t_tree_list *tree, t_env    **env)
{
    int ret;

    ret = 0;
    if (tree->type == comnd)
    {
        ret = is_builtin(tree);
        if (ret != -1)
            ret = execute_builtins(tree, env, ret);
        else
        {
           ret = exec_bin(tree->cmd->cmd, *env);
        }
    }
    // else if (tree->type == PIPE)
    //     //executepipes
    // else if (tree->type == INredirection || tree->type == OUTredirection)
    //     //execute redirections
    // else if (tree->type == HEREdocument)
    //     // execute heredoc
    // else if (tree->type == OUTappend)
    //     // execute append
    return (ret);
}

