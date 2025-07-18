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


int prepare_heredoc(t_tree_list *tree, char **env)
{
    t_tokenlist *curr;

    if (tree->type == comnd && tree->cmd && tree->cmd->in)
    {
        curr = tree->cmd->in;
        while (curr)
        {
            if (curr->type == HEREdocument)
            {
                if (heredoc_redir(curr, env) == -1)
                    return (-1);
            }
            curr = curr->next;
        }        
    }
    if (tree->type == PIPE)
    {
        if (prepare_heredoc(tree->left, env) == -1)
            return (-1);
        if (prepare_heredoc(tree->right, env) == -1)
            return (-1);
    }
    return (0);
}

int execute(t_tree_list *tree, t_env **env)
{
    int ret = 0;
    
    // if (prepare_heredoc(tree) == -1)
    //     return (-1);
    if (tree->type == comnd)
    {
        if (tree->cmd)
        {
            if ((tree->cmd->out && (tree->cmd->out->type == OUTredirection || tree->cmd->out->type == OUTappend)) || 
                (tree->cmd->in && (tree->cmd->in->type == INredirection || tree->cmd->in->type == HEREdocument)))
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

