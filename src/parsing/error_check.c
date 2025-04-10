/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:30 by jwardeng          #+#    #+#             */
/*   Updated: 2025/04/10 14:53:26 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// new logic for handling quotes and environment variables
// NEED TO: check memory leaks
// NEED TO: create expand env logic
// NEED TO: handle error returns throughout functions
// NEED TO: norminette & generally structure and name more logically (cmd | quotes | env var)
// NEED TO: go to token_list and norminette tokenize (+ check efficiency)
// NEED TO: go through structs and delete unused vars
// proj-checks: ms1 (safety copy before changes 10.04)
// NEED TO: maybe combine redirect functions in ast_redirect.c

int	token_command(char *value, size_t len)
{
	if (ft_strncmp(value, "cd", len) == 0 || ft_strncmp(value, "\"cd\"",
			len) == 0 || ft_strncmp(value, "\'cd\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo", len) == 0 || ft_strncmp(value,
			"\"echo\"", len) == 0 || ft_strncmp(value, "\'echo\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "echo-n", len) == 0 || ft_strncmp(value,
			"\"echo-n\"", len) == 0 || ft_strncmp(value, "\'echo-n\'",
			len) == 0)
		return (0);
	else if (ft_strncmp(value, "pwd", len) == 0 || ft_strncmp(value, "\"pwd\"",
			len) == 0 || ft_strncmp(value, "\'pwd\'", len) == 0)
		return (0);
	else if (ft_strncmp(value, "env", len) == 0)
		return (0);
	else if (ft_strncmp(value, "export", len) == 0)
		return (0);
	if (ft_strncmp(value, "unset", len) == 0)
		return (0);
	else if (ft_strncmp(value, "exit", len) == 0)
		return (0);
	else
		return (1);
}

void expand_var(char **str, int *i, int *count, char **new)
{
/*     int copy;
    
    copy = (*i); */
    //(check for var)
    // var valid -> add var to new increment i & count
    // var invalid -> just skip and increment i
    printf("easd\n");
    if((*str)[(*i) + 1] == '?')
    //handle ?
    while ((*str)[*i] != '\0' && (*str)[*i] < 123 && (*str)[*i] > 96 && (*str)[*i] < 91 && (*str)[*i] > 47)
    (*i)++;
    (*new)[*count] = 'X';
    (*count)++; 
}

/* void in_DQ(t_token_data **token_data, char **new, int *i, int *count)
{
    quote_status(token_data, (*str)[i]); 
    if ((*str)[i] == '$' && (*str)[i + 1] != '\0' && (*str)[i + 1] != ' ')
    expand_var(str, &i, &count, &new);
    else if ((*str)[i] != '\"')
    new[count++] = (*str)[i];
    i++;
}

void in_SQ()
{
    
}

void in_NQ()
{
    
} */

char* handle_quotes(t_token_data **token_data, char **str)
{
    int i;
    char *new;
    int count;

    i = 0;
    count = 0;
    new = (char*)gc_malloc((*token_data)->gc, PARSING, ft_strlen(*str) * 4);
    if (!new)
    return(NULL);
    quote_status(token_data, (*str)[i]);
    if((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
     i++;
    while ((*str)[i] != '\0')
    {
       while ((*str)[i] != '\0' && (*token_data)->in_DQ == 1)
        {
            quote_status(token_data, (*str)[i]); 
            if ((*str)[i] == '$' && (*str)[i + 1] != '\0' && (*str)[i + 1] != ' ')
            expand_var(str, &i, &count, &new);
            else if ((*str)[i] != '\"')
            new[count++] = (*str)[i];
            i++;
        }
        while ((*str)[i] != '\0' && (*token_data)->in_SQ == 1)
        {
            quote_status(token_data, (*str)[i]);
            if ((*str)[i] != '\'')
            new[count++] = (*str)[i];
            i++;
        }
        while ((*str)[i] != '\0' && (*token_data)->in_SQ == 0 && (*token_data)->in_DQ == 0)
        {
            quote_status(token_data, (*str)[i]);
            if ((*str)[i] == '$' && (*str)[i + 1] != '\0' && (*str)[i + 1] != ' ')
            expand_var(str, &i, &count, &new);
            if((*str)[i] != '\"' && (*str)[i] != '\'')
            {
            new[count] = (*str)[i];
            count++;
            }
            i++;
        }
    }
    if ((*token_data)->in_DQ == 1 || (*token_data)->in_SQ == 1)
    return((ft_perror_parsing(UNCLOSED_QUOTES, "unclosed quotes")), NULL);
    new[count] = '\0';
    return(new);
}

int valid_cmd(t_token_data **token_data, t_ast *node)
{
    node->cmd_path = find_path(node->args[0], (*token_data)->envp);
        if (node->cmd_path == NULL && token_command(node->args[0],
			ft_strlen(node->args[0])) == 1)
	    {
		if ((*token_data)->syntax_error == 0)
			ft_perror_parsing(INVALID_COMMAND, node->args[0]);
		(*token_data)->syntax_error = 1;
        return(1);
	    } 
    return(0);
}

int quotes_var(t_token_data **token_data, t_ast **ast)
{
    t_ast *node;
    int i;

    node = *ast;
    i = 0;
    if (node->type == TOK_COMMAND)
    {
        while (node->args[i] != NULL)
        {
        if ((node->args[i] = handle_quotes(token_data, &node->args[i])) == NULL)
        return(1);
        i++;
        }
        if (valid_cmd(token_data, node) == 1)
        return(1);
    }
    else if (node->type == TOK_APPEND || node->type == TOK_HEREDOC
        || node->type == TOK_REDIRECT_IN || node->type == TOK_REDIRECT_OUT)
        if ((node->file_name = handle_quotes(token_data, &node->file_name)) == NULL)
        return(1);
    if (node->left != NULL)
    quotes_var(token_data, &node->left);
    if (node->right != NULL)
    quotes_var(token_data, &node->right);   
    return(0); 
}
