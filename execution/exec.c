#include "../minishell.h"


// int execute(t_tree_list *tree, t_env    **env)
// {
//     int ret;
//     ret = 0;
//     if (tree->type == comnd)
//      {
//         if (tree->cmd)
//         {
//             if (tree->cmd->out)
//             {
//                 if (tree->cmd->out->type == OUTredirection)
//                     ret = execute_out_redirection(tree, tree->cmd->out->content, env);
//             }
//             if (tree->cmd->in)
//             {
//                 if (tree->cmd->in->type == INredirection)
//                     ret = execute_in_redirection(tree, tree->cmd->in->content, env);
//             }
//         }
//         else
//         {
//             ret = is_builtin(tree);
//             if (ret != -1)
//                 ret = execute_builtins(tree, env, ret);
//             else
//             {
//                 printf("enters here %d \n", global);
//                 ret = exec_bin(tree->cmd->cmd, *env);
//             }
//         }
//     }
//     else if (tree->type == PIPE)
//     {
//      ret = execute_pipe(tree, env);
//     }
//     // else if (tree->type == HEREdocument)
//     //     // execute heredoc
//     // else if (tree->type == OUTappend)
//     //     // execute append
//     return (ret);
// }

int execute(t_tree_list *tree, t_env **env)
{
    int ret = 0;
    
    if (tree->type == comnd)
    {
        if (tree->cmd)
        {
            if ((tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend)) || 
                (tree->cmd->in && tree->cmd->in->type == INredirection ))
                ret = execute_with_redirections(tree, env);
            else
            {
                ret = is_builtin(tree);
                if (ret != -1)
                    ret = execute_builtins(tree, env, ret);
                else
                    ret = exec_bin(tree->cmd->cmd, *env);
            }
        }
    }
    else if (tree->type == PIPE)
        ret = execute_pipe(tree, env);
    return (ret);
}

