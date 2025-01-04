#include "gtk-layer-demo.h"

typedef struct {
    GtkLayerShellEdge edge;
    GtkWindow* layer_window;
} AnchorButtonData;

static void on_anchor_toggled(GtkToggleButton* button, AnchorButtonData* data) {
    gboolean is_anchored = gtk_toggle_button_get_active(button);
    ToplevelData* toplevel_data = g_object_get_data(G_OBJECT(data->layer_window), anchor_edges_key);
    g_return_if_fail(toplevel_data);
    toplevel_data->edges[data->edge] = is_anchored;
    layer_window_update_orientation(data->layer_window);
    gtk_layer_set_anchor(data->layer_window, data->edge, is_anchored);
}

static GtkWidget* anchor_edge_button_new(GtkWindow* layer_window,
                        GtkLayerShellEdge edge,
                        const gboolean defaults[GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER],
                        const char* icon_name,
                        const char* tooltip) {
    GtkWidget* button = gtk_toggle_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(button), icon_name);
    gtk_widget_set_tooltip_text(button, tooltip);
    AnchorButtonData* data = g_new0(AnchorButtonData, 1);
    *data = (AnchorButtonData) {
        .edge = edge,
        .layer_window = layer_window,
    };
    g_object_set_data_full(G_OBJECT(button), "clicked_signal_data", data, (GDestroyNotify)g_free);
    g_signal_connect(button, "clicked", G_CALLBACK(on_anchor_toggled), data);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), defaults[edge]);
    return button;
}

GtkWidget* anchor_control_new(GtkWindow* layer_window, const gboolean default_anchors[GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER]) {
    GtkWidget* outside_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(outside_vbox, TRUE);
    GtkWidget* outside_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(outside_vbox), outside_hbox); {
        GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
        gtk_box_append(GTK_BOX(outside_hbox), hbox); {
            GtkWidget* vbox = gtk_center_box_new();
            gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
            gtk_box_append(GTK_BOX(hbox), vbox); {
                GtkWidget* button = anchor_edge_button_new(layer_window, GTK_LAYER_SHELL_EDGE_LEFT, default_anchors, "go-previous", "Anchor left");
                gtk_center_box_set_center_widget(GTK_CENTER_BOX(vbox), button);
            }
        }{
            GtkWidget* center_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
            gtk_box_append(GTK_BOX(hbox), center_vbox); {
                GtkWidget* button = anchor_edge_button_new(layer_window, GTK_LAYER_SHELL_EDGE_TOP, default_anchors, "go-up", "Anchor top");
                gtk_box_append(GTK_BOX(center_vbox), button);
            }{
                GtkWidget* button = anchor_edge_button_new(layer_window, GTK_LAYER_SHELL_EDGE_BOTTOM, default_anchors, "go-down", "Anchor bottom");
                gtk_box_append(GTK_BOX(center_vbox), button);
            }
        }{
            GtkWidget* vbox = gtk_center_box_new();
            gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
            gtk_box_append(GTK_BOX(hbox), vbox); {
                GtkWidget* button = anchor_edge_button_new(layer_window, GTK_LAYER_SHELL_EDGE_RIGHT, default_anchors, "go-next", "Anchor right");
                gtk_center_box_set_center_widget(GTK_CENTER_BOX(vbox), button);
            }
        }
    }

    return outside_vbox;
}
