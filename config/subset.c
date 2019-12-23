/**
 * @file
 * Subset of config items
 *
 * @authors
 * Copyright (C) 2019 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page config_subset Subset of config items
 *
 * Subset of config items
 */

#include "config.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include "mutt/mutt.h"
#include "config/lib.h"
#include "subset.h"
#include "dump.h"
#include "set.h"

/**
 * get_base - Find the root Config Item
 * @param he Config Item to examine
 * @retval ptr Root Config Item
 *
 * Given an inherited HashElem, find the HashElem representing the original
 * Config Item.
 */
static struct HashElem *get_base(struct HashElem *he)
{
  if (!(he->type & DT_INHERITED))
    return he;

  struct Inheritance *i = he->data;
  return get_base(i->parent);
}

/**
 * cs_subset_free - Free a Config Subset
 * @param ptr Subset to free
 *
 * @note Config items matching this Subset will be freed
 */
void cs_subset_free(struct ConfigSubset **ptr)
{
  if (!ptr || !*ptr)
    return;

  struct ConfigSubset *sub = *ptr;

  if (sub->name)
  {
    char scope[256];
    if (sub->parent && sub->parent->name)
      snprintf(scope, sizeof(scope), "%s:%s:", sub->parent->name, sub->name);
    else
      snprintf(scope, sizeof(scope), "%s:", sub->name);

    // We don't know if any config items have been set,
    // so search for anything with a matching scope.
    struct HashElem **list = get_elem_list(sub->cs);
    for (size_t i = 0; list[i]; i++)
    {
      const char *item = list[i]->key.strkey;
      if (mutt_str_startswith(item, scope, CASE_MATCH) != 0)
      {
        cs_uninherit_variable(sub->cs, item);
      }
    }
    FREE(&list);
  }

  notify_free(&sub->notify);
  FREE(&sub->name);
  FREE(ptr);
}

/**
 * cs_subset_new - Create a new Config Subset
 * @param name   Name for this Subset
 * @param sub_parent Parent Subset
 * @param not_parent Parent Notification
 * @retval ptr New Subset
 *
 * @note The name will be combined with the parent's names
 */
struct ConfigSubset *cs_subset_new(const char *name, struct ConfigSubset *sub_parent, struct Notify *not_parent)
{
  struct ConfigSubset *sub = mutt_mem_calloc(1, sizeof(*sub));

  if (sub_parent)
  {
    sub->parent = sub_parent;
    sub->cs = sub_parent->cs;
  }

  if (name)
  {
    char scope[256];

    if (sub_parent && sub_parent->name)
      snprintf(scope, sizeof(scope), "%s:%s", sub_parent->name, name);
    else
      mutt_str_strfcpy(scope, name, sizeof(scope));

    sub->name = mutt_str_strdup(scope);
  }

  sub->notify = notify_new();
  notify_set_parent(sub->notify, not_parent);

  return sub;
}

/**
 * cs_subset_lookup - Find an inherited config item
 * @param sub  Subset to search
 * @param name Name of Config item to find
 * @retval ptr HashElem of the config item
 */
struct HashElem *cs_subset_lookup(const struct ConfigSubset *sub, const char *name)
{
  if (!sub || !name)
    return NULL;

  char scope[256];
  if (sub->name)
    snprintf(scope, sizeof(scope), "%s:%s", sub->name, name);
  else
    mutt_str_strfcpy(scope, name, sizeof(scope));

  return cs_get_elem(sub->cs, scope);
}

/**
 * cs_subset_native_get - Natively get the value of an inherited config item
 * @param sub Config Subset
 * @param he  HashElem representing config item
 * @param err Buffer for error messages
 * @retval intptr_t Native pointer/value
 * @retval INT_MIN  Error
 */
intptr_t cs_subset_native_get(const struct ConfigSubset *sub,
                              struct HashElem *he, struct Buffer *err)
{
  if (!sub || !he)
    return INT_MIN;

  return cs_he_native_get(sub->cs, he, err);
}

/**
 * cs_subset_native_set - Natively set the value of an inherited config item
 * @param sub   Config Subset
 * @param he    HashElem representing config item
 * @param value Native pointer/value to set
 * @param err   Buffer for error messages
 * @retval num Result, e.g. #CSR_SUCCESS
 */
int cs_subset_native_set(const struct ConfigSubset *sub, struct HashElem *he,
                         intptr_t value, struct Buffer *err)
{
  if (!sub || !he)
    return CSR_ERR_CODE;

  int rc = cs_he_native_set(sub->cs, he, value, err);
  if (!(rc & CSR_SUC_NO_CHANGE))
  {
    struct HashElem *he_base = get_base(he);
    struct EventConfig ec = { sub, he_base->key.strkey, he };
    notify_send(sub->notify, NT_CONFIG, NT_CONFIG_SET, &ec);
  }
  return rc;
}

/**
 * cs_subset_reset - Reset an inherited config item to its parent value
 * @param sub  Config Subset
 * @param he   HashElem representing config item
 * @param err  Buffer for error messages
 * @retval num Result, e.g. #CSR_SUCCESS
 */
int cs_subset_reset(const struct ConfigSubset *sub, struct HashElem *he, struct Buffer *err)
{
  if (!sub || !he)
    return CSR_ERR_CODE;

  return cs_he_reset(sub->cs, he, err);
}

/**
 * cs_subset_string_get - Get an inherited config item as a string
 * @param sub    Config Subset
 * @param he     HashElem representing config item
 * @param result Buffer for results or error messages
 * @retval num Result, e.g. #CSR_SUCCESS
 */
int cs_subset_string_get(const struct ConfigSubset *sub, struct HashElem *he,
                         struct Buffer *result)
{
  if (!sub || !he)
    return CSR_ERR_CODE;

  return cs_he_string_get(sub->cs, he, result);
}

/**
 * cs_subset_string_set - Set an inherited config item by string
 * @param sub   Config Subset
 * @param he    HashElem representing config item
 * @param value Value to set
 * @param err   Buffer for error messages
 * @retval num Result, e.g. #CSR_SUCCESS
 */
int cs_subset_string_set(const struct ConfigSubset *sub, struct HashElem *he,
                         const char *value, struct Buffer *err)
{
  if (!sub || !he)
    return INT_MIN;

  return cs_he_string_set(sub->cs, he, value, err);
}

/**
 * cs_subset_create_var - Create an inherited config item
 * @param sub  Config Subset
 * @param name Name of Config item to create
 * @param err  Buffer for error messages
 * @retval ptr HashElem of the config item
 */
struct HashElem *cs_subset_create_var(const struct ConfigSubset *sub,
                                      const char *name, struct Buffer *err)
{
  if (!sub || !name)
    return NULL;

  // Check if name already exists
  struct HashElem *he = cs_subset_lookup(sub, name);
  if (he)
    return he;

  // Create parent before creating name
  he = cs_subset_create_var(sub->parent, name, err);
  if (!he)
    return NULL;

  char scope[256];
  snprintf(scope, sizeof(scope), "%s:%s", sub->name, name);

  return cs_inherit_variable(sub->cs, he, scope);
}

intptr_t cs_subset_native_get2(const struct ConfigSubset *sub, const char *name, struct Buffer *err)
{
  if (!sub || !name)
    return INT_MIN;

  struct HashElem *he = cs_subset_lookup(sub, name);
  if (he)
    return cs_subset_native_get(sub, he, err);

  return cs_subset_native_get2(sub->parent, name, err);
}

struct HashElem *create_inheritance(const struct ConfigSubset *sub, const char *name)
{
  if (!sub || !name)
    return NULL;

  struct HashElem *he = cs_subset_lookup(sub, name);
  if (he)
    return he;

  // Create parent before creating name
  he = create_inheritance(sub->parent, name);

  char scope[256];
  snprintf(scope, sizeof(scope), "%s:%s", sub->name, name);
  return cs_inherit_variable(sub->cs, he, scope);
}

int cs_subset_native_set2(const struct ConfigSubset *sub, const char *name,
                         intptr_t value, struct Buffer *err)
{
  if (!sub || !name)
    return CSR_ERR_CODE;

  struct HashElem *he = create_inheritance(sub, name);
  if (!he)
    return CSR_ERR_CODE;

  int rc = cs_he_native_set(sub->cs, he, value, err);
  if (!(rc & CSR_SUC_NO_CHANGE))
  {
    struct HashElem *he_base = get_base(he);
    struct EventConfig ec = { sub, he_base->key.strkey, he };
    notify_send(sub->notify, NT_CONFIG, NT_CONFIG_SET, &ec);
  }
  return rc;
}

