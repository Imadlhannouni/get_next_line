/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:46:23 by ilhannou          #+#    #+#             */
/*   Updated: 2024/12/08 13:25:29 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_line_from_reminder(t_list **reminder)
{
	char	*line;
	t_list	*temp;

	if (*reminder)
	{
		line = ft_strdup((*reminder)->content);
		if (!line)
			return (NULL);
		temp = *reminder;
		*reminder = (*reminder)->next;
		free(temp->content);
		free(temp);
		return (line);
	}
	return (NULL);
}

static void	update_reminder(char *line, t_list **reminder)
{
	char	*newline_pos;
	char	*new_reminder;

	newline_pos = ft_strchr(line, '\n');
	if (newline_pos)
	{
		new_reminder = ft_strdup(newline_pos + 1);
		if (!new_reminder)
			return ;
		*(newline_pos + 1) = '\0';
		*reminder = malloc(sizeof(t_list));
		if (!(*reminder))
		{
			free(new_reminder);
			return ;
		}
		(*reminder)->content = new_reminder;
		(*reminder)->next = NULL;
	}
	else
		*reminder = NULL;
}

static char	*read_and_assemble_line(int fd, char *line)
{
	ssize_t	bytes_r;
	char	*buffer;
	char	*temp;

	buffer = malloc(BUFFER_SIZE + 1);
	while (!ft_strchr(line, '\n'))
	{
		bytes_r = read(fd, buffer, BUFFER_SIZE);
		if (bytes_r < 0 || (!bytes_r && !*line))
			return (free(buffer), free(line), NULL);
		if (bytes_r == 0)
			break ;
		buffer[bytes_r] = '\0';
		temp = ft_strjoin(line, buffer);
		free(line);
		if (!temp)
			return (free(buffer), NULL);
		line = temp;
	}
	free(buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static t_list	*reminder;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = get_line_from_reminder(&reminder);
	if (!line)
		line = ft_strdup("");
	line = read_and_assemble_line(fd, line);
	if (!line)
		return (NULL);
	update_reminder(line, &reminder);
	return (line);
}
