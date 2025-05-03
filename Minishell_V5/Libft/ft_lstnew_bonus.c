/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:13:45 by nimorel           #+#    #+#             */
/*   Updated: 2024/11/13 10:45:46 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_lst;

	new_lst = malloc(sizeof(t_list));
	if (!new_lst)
		return (NULL);
	new_lst->content = content;
	new_lst->next = NULL;
	return (new_lst);
}
/*int main(void)
{
    char *content;
    t_list *new_node;

    content = "Hello, World!";
	new_node = ft_lstnew(content);
	if (new_node == NULL)
	{
        printf("Erreur : le nœud n'a pas pu être créé.\n");
        return (1);
    }
    if (new_node->content == content)
        printf("Contenu correct : %s\n", (char *)new_node->content);
    else
        printf("Erreur : contenu incorrect.\n");
    if (new_node->next == NULL)
        printf("Le pointeur 'next' est correctement initialisé à NULL.\n");
    else
        printf("Erreur : le pointeur 'next' n'est pas NULL.\n");
    free(new_node);

    return (0);
}
*/
