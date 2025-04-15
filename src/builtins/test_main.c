/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyang <xueyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:05:01 by xueyang           #+#    #+#             */
/*   Updated: 2025/04/15 22:38:58 by xueyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// test env
// int	main(int argc, char **argv, char **envp)
// {
// 	t_gc	*gc;
// 	t_env	*top;
// 	char	*args[20];
// 	char	*arg_test1[5];
// 	char	*arg_test2[5];
// 	int		i;
// 	int		j;

// 	if (argc == 1)
// 		return (0);
// 	gc = init_gc();
// 	top = init_env(envp, gc);
// 	i = 1;
// 	while (argv[i])
// 	{
// 		args[i - 1] = ft_strdup(argv[i]);
// 		i++;
// 	}
// 	args[i - 1] = NULL;
// 	j = 0;
// 	while(args[j])
// 	{
// 		printf("args: %s\n", args[j]);
// 		j++;
// 	}
// 	arg_test1[0] = "export";
// 	arg_test1[1] = NULL;
// 	arg_test2[0] = "unset";
// 	arg_test2[1] = "HAHA";
// 	arg_test2[2] = NULL;
// 	while (1)
// 	{
// 		ft_export(top, args, gc);
// 		ft_export(top, arg_test1, gc);
// 		ft_unset(top, arg_test2);
// 		ft_export(top, arg_test1, gc);
// 		break;
// 	}
// 	return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	t_gc	*gc;
	t_env	*top;
	char	*args[20];
	char	*arg_test1[5];
	int		i;
	int		j;

	if (argc == 1)
		return (0);
	gc = init_gc();
	top = init_env(envp, gc);
	i = 1;
	while (argv[i])
	{
		args[i - 1] = ft_strdup(argv[i]);
		i++;
	}
	args[i - 1] = NULL;
	j = 0;
	while(args[j])
	{
		printf("args: %s\n", args[j]);
		j++;
	}
	arg_test1[0] = "pwd";
	arg_test1[1] = NULL;
	while (1)
	{
		ft_cd(args, top, gc);
		ft_pwd(arg_test1);
		break;
	}
	return (0);
}