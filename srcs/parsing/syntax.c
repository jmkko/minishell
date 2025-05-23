/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhervoch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:51:50 by jhervoch          #+#    #+#             */
/*   Updated: 2025/02/26 18:58:31 by npolack          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	parenthesis_syntax(t_token *prev, t_token *curr)
{
	int	error;

	error = 0;
	if (!prev)
	{
		if (!ft_strcmp(curr->input, "("))
			return (0);
		else
			return (1);
	}
	if (!ft_strcmp(curr->input, "("))
	{
		if (prev->type == CMD || !ft_strcmp(prev->input, ")"))
			error = 1;
	}
	else if (!ft_strcmp(curr->input, ")"))
	{
		if (!ft_strcmp(prev->input, ")"))
			error = 0;
		else if (prev->type == OPERATOR || prev->type == PIPE)
			error = 1;
	}
	return (error);
}

static int	operator_syntax(t_token *prev)
{
	int	error;

	if (!prev)
		return (1);
	error = 0;
	if (!ft_strcmp(prev->input, ")"))
		error = 0;
	else if (prev->type == PIPE || prev->type == OPERATOR)
		error = 1;
	return (error);
}

static int	rdir_syntax(t_token *curr)
{
	int		i;
	char	*str;
	int		error;

	str = curr->input;
	error = 0;
	while (*str && error != 2)
	{
		if (*str == '>' || *str == '<')
		{
			i = rdir_len(str);
			i = check_rdir_name(str, &error, i);
			str += i;
		}
		else if (ft_isquote(*str))
			str += skip_quote(str, *str);
		else
			str++;
	}
	return (error);
}

static int	cmd_syntax(t_token *prev, t_token *curr)
{
	int	error;

	error = 0;
	if (prev)
	{
		if (prev->type == CMD)
			return (1);
		if (!ft_strcmp(prev->input, ")"))
		{
			ft_printf(2, "Use parenthesis for priority ONLY\n");
			return (1);
		}
	}
	error = rdir_syntax(curr);
	return (error);
}

int	catch_syntax_error(t_token *prev, t_token *curr)
{
	int	error;

	error = 0;
	if (!prev && curr)
	{
		if (curr->type != CMD && ft_strcmp(curr->input, "("))
			error = 1;
		else if (!ft_strcmp(curr->input, "("))
			return (0);
		else
			error = cmd_syntax(prev, curr);
	}
	else if (!ft_strcmp(curr->input, "&"))
		error = 1;
	else if (!ft_strcmp(curr->input, ")") || !ft_strcmp(curr->input, "("))
		error = parenthesis_syntax(prev, curr);
	else if (curr->type == OPERATOR || curr->type == PIPE)
		error = operator_syntax(prev);
	else if (curr->type == CMD)
		error = cmd_syntax(prev, curr);
	if (error)
		ft_printf(2, SYNTERR);
	return (error);
}
