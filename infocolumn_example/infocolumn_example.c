
/*
 * Copyright (C) 2014 Alexander van der Meij <alexandervdm@gliese.me>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-column-provider.h>
#include <libcaja-extension/caja-info-provider.h>

typedef struct {
    GObject parent;
} InfoColumnExample;

typedef struct {
    GObjectClass parent_class;
} InfoColumnExampleClass;

typedef struct {
    GClosure *update_complete;
    CajaInfoProvider *provider;
    CajaFileInfo *file;
    int operation_handle;
    gboolean cancelled; 
} UpdateHandle;


static GObjectClass *parent_class = NULL;
static GType provider_types[1];
static GType infocolumn_example_type;


static void infocolumn_example_register_type (GTypeModule *module);
GType infocolumn_example_get_type (void);

static void infocolumn_example_class_init (InfoColumnExampleClass *class);
static void infocolumn_example_instance_init (InfoColumnExample *self);

static void column_provider_iface_init (CajaColumnProviderIface *iface);
static void info_provider_iface_init (CajaInfoProviderIface *iface);


static GList * infocolumn_example_get_columns (CajaColumnProvider *provider);
static CajaOperationResult
infocolumn_example_update_file_info (CajaInfoProvider *provider,
                                     CajaFileInfo *file,
                                     GClosure *update_complete,
                                     CajaOperationHandle **handle);
static void infocolumn_example_cancel_update (CajaInfoProvider *provider,
                                              CajaOperationHandle *handle);

/* Extension interface: */

void
caja_module_initialize (GTypeModule  *module) 
{
	infocolumn_example_register_type (module);

	provider_types[0] = infocolumn_example_get_type ();
}

void 
caja_module_shutdown (void)
{
	// Any module-specific shutdown:
}

void
caja_module_list_types (const GType **types,
			    int *num_types)
{
	*types = provider_types;
	*num_types = G_N_ELEMENTS (provider_types);
}


static void
infocolumn_example_register_type (GTypeModule *module)
{
        static const GTypeInfo info = {
                sizeof (InfoColumnExampleClass),
                (GBaseInitFunc) NULL,
                (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) infocolumn_example_class_init,
                NULL,
                NULL,
                sizeof (InfoColumnExample),
                0,
                (GInstanceInitFunc) infocolumn_example_instance_init,
        };

    static const GInterfaceInfo column_provider_iface_info = {
        (GInterfaceInitFunc)column_provider_iface_init,
        NULL,
        NULL
    };

	static const GInterfaceInfo info_provider_iface_info = {
		(GInterfaceInitFunc)info_provider_iface_init,
		NULL,
		NULL
	};

	infocolumn_example_type = g_type_module_register_type (module,
							  G_TYPE_OBJECT,
							  "InfoColumnExample",
							  &info, 0);

    // add interface:
    g_type_module_add_interface (module,
				 infocolumn_example_type,
				 CAJA_TYPE_COLUMN_PROVIDER,
				 &column_provider_iface_info);

	g_type_module_add_interface(module,
                infocolumn_example_type,
                CAJA_TYPE_INFO_PROVIDER,
                &info_provider_iface_info);
}

GType
infocolumn_example_get_type (void)
{
	return infocolumn_example_type;
}


/* Initiation functions: */

static void
infocolumn_example_class_init (InfoColumnExampleClass *class)
{
    parent_class = g_type_class_peek_parent (class);
}

static void
infocolumn_example_instance_init (InfoColumnExample *self)
{
  g_message ("Initializing InfoColumnExample extension...");
}

static void column_provider_iface_init (CajaColumnProviderIface *iface)
{
    iface->get_columns = infocolumn_example_get_columns;
}

static void info_provider_iface_init (CajaInfoProviderIface *iface)
{
    iface->update_file_info = infocolumn_example_update_file_info;
    iface->cancel_update = infocolumn_example_cancel_update;
}

/* Info Provider specific functions: */
                                                                             
static void
infocolumn_example_cancel_update (CajaInfoProvider *provider,
                                  CajaOperationHandle *handle)
{
        UpdateHandle *update_handle = (UpdateHandle*)handle;
        update_handle->cancelled = TRUE;
}

static CajaOperationResult
infocolumn_example_update_file_info (CajaInfoProvider *provider,
                                     CajaFileInfo *file,
                                     GClosure *update_complete,
                                     CajaOperationHandle **handle)
{
    /* This example only implements the most basic form of the 
     * update-file-info mechanism. For real-world applications, you
     * should look into caching previously computed results.
     * More information here: {wiki-link} */ 

	CajaFileInfoIface *file_iface = CAJA_FILE_INFO_GET_IFACE(file);

    if (file_iface->is_directory (file))
    {
        caja_file_info_add_emblem (file, "development");
        caja_file_info_add_string_attribute(file, 
                                            "InfoColumnExample::data", 
                                            "directory");
    }
    else
    {
        caja_file_info_add_emblem (file, "art");
        caja_file_info_add_string_attribute(file, 
                                            "InfoColumnExample::data", 
                                            "file");    
    }
    return CAJA_OPERATION_COMPLETE;
}


/* Column Provider specific functions: */

static GList *
infocolumn_example_get_columns (CajaColumnProvider *provider)
{
	CajaColumn *column;
	GList *ret;

	column = caja_column_new ("InfoColumnExample::data_column",
                              "InfoColumnExample::data",
                              "InfoColumnExample Data",
                              "Information from the InfoColumnExample");
    ret = g_list_append (NULL, column);
	return ret;
}
