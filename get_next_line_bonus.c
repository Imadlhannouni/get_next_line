/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:45:32 by ilhannou          #+#    #+#             */
/*   Updated: 2024/12/06 15:51:29 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

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

char	*read_and_assemble_line(int fd, char *line)
{
	ssize_t	bytes_r[1024];
	char	*buffer;
	char	*temp;

	buffer = malloc(BUFFER_SIZE + 1);
	while (!ft_strchr(line, '\n'))
	{
		bytes_r[fd] = read(fd, buffer, BUFFER_SIZE);
		if (bytes_r[fd] < 0 || (!bytes_r[fd] && !*line))
			return (free(buffer), free(line), NULL);
		if (bytes_r[fd] == 0)
			break ;
		buffer[bytes_r[fd]] = '\0';
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
	static t_list	*reminder[1024];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = get_line_from_reminder(&reminder[fd]);
	if (!line)
		line = ft_strdup("");
	line = read_and_assemble_line(fd, line);
	if (!line)
		return (NULL);
	update_reminder(line, &reminder[fd]);
	return (line);
}

// int main(void)
// {
//     int fd1, fd2, fd3;
//     char *line;
//     fd1 = open("file1.txt", O_RDONLY);
//     fd2 = open("file2.txt", O_RDONLY);
//     fd3 = open("file3.txt", O_RDONLY);
//     if (fd1 < 0 || fd2 < 0 || fd3 < 0)
//     {
//         perror("Error opening file");
//         return (1);
//     }
//     while (1)
//     {
//         line = get_next_line(fd1);
//         if (line)
//         {
//             printf("File 1: %s", line);
//             free(line);
//         }
//         else
//         {
//             printf("File 1: EOF\n");
//         }
//         line = get_next_line(fd2);
//         if (line)
//         {
//             printf("File 2: %s", line);
//             free(line);
//         }
//         else
//         {
//             printf("File 2: EOF\n");
//         }
//         line = get_next_line(fd3);
//         if (line)
//         {
//             printf("File 3: %s", line);
//             free(line);
//         }
//         else
//         {
//             printf("File 3: EOF\n");
//         }
//         if (!line)
//             break;
//     }
//     close(fd1);
//     close(fd2);
//     close(fd3);
//     return (0);
// }