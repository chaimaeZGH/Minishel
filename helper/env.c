#include "../minishell.h"

t_env   *create_node(char    *key, char  *value)
{
    t_env   *new_node;

    new_node = malloc(sizeof(t_env));
    if (key)
        new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = NULL;
    new_node->next = NULL;
    return (new_node);
}

void   add_front(t_env **head, t_env    *new_node)
{
    if (*head == NULL)
    {
        *head = new_node;
        return ;
    }
    new_node->next = *head;
    *head = new_node;
}

t_env   *copy_env(char **env)
{
    int i;
    t_env *head;
    char  *eq_pos;
    char    *key;
    char    *value;
    t_env   *new_node;

    head = NULL;
    i = 0;
    while (env[i])
    {
        eq_pos = ft_strchr(env[i], '=');
        if (eq_pos && eq_pos != env[i])
        {   
            *eq_pos = '\0';
            key = env[i];
            value = eq_pos + 1;
            new_node = create_node(key, value);
            add_front(&head, new_node);
            *eq_pos = '=';
        }
        i++;
    }
    return (head);
}

t_env   *find_key(t_env *head, char *key)
{
    while (head)
    {
        if (strcmp(head->key, key) == 0)
            return (head);
        head = head->next;
    }
    return (NULL);
}

void    set_env_value(t_env **head, char *key, char *value)
{
    t_env   *node;
    t_env   *new_node;

    new_node = NULL;
    node = find_key(*head, key);
    if (node)
    {
        if (value)
        {
            free(node->value);
            node->value = ft_strdup(value);
        }
    }
    else
    {
        new_node = create_node(key, value);
        add_front(head, new_node);
    }
}
void print_env_list(t_env *head)
{
    t_env *current = head;
    
    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

void    ft_swap(char **key, char **s_key)
{
    char *tmp;

    tmp = *key;
    *key = *s_key;
    *s_key = tmp;
}

t_env   *sort(t_env *env)
{
    t_env *curr;
    t_env  *tmp;

    if (!env || !env->next)
        return (env);
    curr = env;
    while (curr->next != NULL)
    {
        tmp = curr->next;
        while (tmp != NULL)
        {
            if (strcmp(curr->key, tmp->key) > 0)
            {
                ft_swap(&curr->key, &tmp->key);
                ft_swap(&curr->value, &tmp->value);
            }
            tmp = tmp->next;
        }
        curr = curr->next;
    }
    return (env);
}

t_env   *copy_env_list(t_env *env)
{
    t_env   *new_list;
    t_env   *current;
    t_env   *new_node;

    new_list = NULL;
    current = env;
    while (current)
    {
        new_node = create_node(current->key, current->value);
        add_front(&new_list, new_node);
        current = current->next;
    }
    return (new_list);
}

// int main (int ac, char **av, char **env)
// {
//     t_env *str;

//     str = copy_env(env);
//     print_env_list(str);
//     return (0);
// }