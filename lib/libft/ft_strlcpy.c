/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscarmathot <oscarmathot@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:02:56 by omathot           #+#    #+#             */
/*   Updated: 2023/04/07 23:55:57 by oscarmathot      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	n;
	size_t	len;
	size_t	size;

	if (dstsize > 0)
		size = dstsize - 1;
	else
		size = 0;
	len = ft_strlen(src);
	n = 0;
	while (*src && n < size)
	{
		*dst = *src;
		dst++;
		src++;
		n++;
	}
	if (dstsize > 0)
		*dst = '\0';
	return (len);
}
