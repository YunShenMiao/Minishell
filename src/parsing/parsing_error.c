/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwardeng <jwardeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:55:26 by jwardeng          #+#    #+#             */
/*   Updated: 2025/03/23 15:52:12 by jwardeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// >>> when no quotes and spaces 
// (eg. echo miao> >> output.txt || echo miao >>>output.txt)
// or: echo miao> > output.txt
// (zsh: parse error near '>>')

/* int    parsing_error(t_token **token_list)
{
    t_token *temp;

    if (*token_list == NULL)
    return(-1);
    temp = (*token_list);
    while(temp != NULL && temp->next != NULL)
    {
        printf("miao1\n");
        if (temp->type == TOK_REDIRECT_IN || temp->type == TOK_REDIRECT_OUT
            || temp->type == TOK_APPEND || temp->type == TOK_HEREDOC)
        {
            printf("miao2\n");
            if ((temp->next->type == TOK_REDIRECT_IN || temp->next->type == TOK_REDIRECT_OUT
                || temp->next->type == TOK_APPEND || temp->next->type == TOK_HEREDOC))
            return(printf("parse error: REDIRECT/APPEND/HEREDOC\n"), -1);
        }
        printf("miao3\n");
        if(temp->next == NULL)
        break;
        temp = temp->next;
    }
    printf("miao4\n");
return(1);
} */