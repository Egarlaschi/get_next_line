/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egarlasc <egarlasc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 13:03:26 by egarlasc          #+#    #+#             */
/*   Updated: 2026/06/15 14:15:41 by egarlasc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*ft_strchr(const char *s, int c);
int		ft_strlen(const char *str);
int		ft_strlcpy(char *dest, const char *src, int size);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*read_and_stash(int fd, char *stash);
char	*line_extractor(char *stash);
char	*stash_cleaner(char *stash);
char	*get_next_line(int fd);

#endif
