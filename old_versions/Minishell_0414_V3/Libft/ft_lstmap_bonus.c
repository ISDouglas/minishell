/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:08:19 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/16 13:26:17 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*new_content;

	new_list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
/*
void	*double_content(void *content)
{
	int	*new_value = malloc(sizeof(int));
	if (!new_value)
		return (NULL);
	*new_value = (*(int *)content) * 2;
	return (new_value);
}
void	del(void *content)
{
	free(content);
}

int	main(void)
{
	t_list	*lst = NULL;
	t_list	*new_lst = NULL;
	int	val1 = 1, val2 = 2, val3 = 3;

	ft_lstadd_back(&lst, ft_lstnew(&val1));
	ft_lstadd_back(&lst, ft_lstnew(&val2));
	ft_lstadd_back(&lst, ft_lstnew(&val3));
	printf("Liste originale : ");
	for (t_list *tmp = lst; tmp; tmp = tmp->next)
		printf("%d ", *(int *)tmp->content);
	printf("\n");
	new_lst = ft_lstmap(lst, &double_content, &del);
	printf("Nouvelle liste (doublée) : ");
	for (t_list *tmp = new_lst; tmp; tmp = tmp->next)
		printf("%d ", *(int *)tmp->content);
	printf("\n");
	ft_lstclear(&lst, &del);
	ft_lstclear(&new_lst, &del);
	return (0);
}
*/
