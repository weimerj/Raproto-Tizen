/*
 * utility.c
 *
 *  Created on: Feb 15, 2021
 *      Author: weimer
 */


#include "utility.h"




void
error_msg(int rc, char const *caller_name, char const *id) {
	dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s : [%d, %s]",caller_name, id, rc, get_error_message(rc));
	return;
}


static void
update_theme_frame(frame_data_s *frame, int r, int g, int b){
	if ((frame->gl == NULL) || (frame->bg == NULL)) return;

	int default_theme[3] = {RAPROTO_COLOR_BACKGROUND_DEFAULT};

	if ((r == default_theme[0]) && (g == default_theme[1]) && (b == default_theme[2])){
		elm_genlist_realized_items_update(frame->gl);
		elm_bg_color_set(frame->bg,r,g,b);
		elm_object_style_set(frame->gl, "default");
	} else {
		elm_genlist_realized_items_update(frame->gl);
		elm_bg_color_set(frame->bg,r,g,b);
		elm_object_style_set(frame->gl, "transparent");
	}
}


static void
update_theme_all(int r, int g, int b, app_data_s *ad){
	update_theme_frame(&(ad->frame_main), r, g, b);
	update_theme_frame(&(ad->frame_settings), r, g, b);
	update_theme_frame(&(ad->frame_mqtt), r, g, b);
	update_theme_frame(&(ad->frame_config), r, g, b);
	update_theme_frame(&(ad->frame_about), r, g, b);
}



void
update_theme(app_data_s *ad){

	dlog_print(DLOG_INFO, LOG_TAG, "update theme called");

	static int prev_state = 0;
	int current_state;

	if (ad->service.error) current_state = -1;
	else if (ad->service.logging) current_state = 1;
	else current_state = 0;

	if (current_state == prev_state) return;
	else prev_state = current_state;

	if (ad->service.error) update_theme_all(RAPROTO_COLOR_BACKGROUND_ERROR, ad);
	else if (ad->service.logging) update_theme_all(RAPROTO_COLOR_BACKGROUND_LOGGING, ad);
	else update_theme_all(RAPROTO_COLOR_BACKGROUND_DEFAULT, ad);

}



void
update_frame(Evas_Object *gl, int num_entries) {

	Elm_Object_Item *it;

	for (int j = 0; j <= num_entries; j++) {
		it = elm_genlist_nth_item_get(gl, j);
		elm_genlist_item_update(it);
	}

}



/*
 * Release list item data structure
 */
static void
gl_del_cb(void *data, Evas_Object *obj)
{
	list_item_data_s *lid = (list_item_data_s*)data;
	lid->index = 0;
	lid->settings = NULL;
	lid->service = NULL;
	free(lid);
}



Evas_Object*
get_genlist_item_2text1check(Evas_Object *obj, const char *text, const char *subtext, Evas_Object **check){

	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};

	Evas_Object *bg = elm_bg_add(obj);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_min_set(bg, 100, 100);
	evas_object_show(bg);

	Evas_Object *hbox = elm_box_add(bg);
	evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_horizontal_set(hbox, EINA_TRUE);
	elm_object_content_set(bg, hbox);
	evas_object_show(hbox);


	Evas_Object *box = elm_box_add(hbox);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(box);
	elm_box_pack_end(hbox, box);

	Evas_Object *label = elm_label_add(box);
	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
	elm_object_text_set(label, str);
	evas_object_show(label);
	elm_box_pack_end(box, label);

	Evas_Object *sublabel = elm_label_add(box);
	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_sub, subtext);
	elm_object_text_set(sublabel, str);
	evas_object_show(sublabel);
	elm_box_pack_end(box, sublabel);

	//Evas_Object *check;
	*check = elm_check_add(hbox);
	evas_object_show(*check);
	elm_box_pack_end(hbox,*check);

	Evas_Object *spacer = evas_object_rectangle_add(hbox);
	evas_object_size_hint_min_set(spacer, 50, 0);
	evas_object_color_set(spacer, 0, 0, 0, 0);
	elm_box_pack_end(hbox, spacer);

	return bg;
}


Evas_Object*
get_genlist_item_1text(Evas_Object *obj, const char *text){

	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};

	Evas_Object *bg = elm_bg_add(obj);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_min_set(bg, 100, 100);
	//elm_bg_color_set(bg, RAPROTO_BACKGROUND_ERROR);
	evas_object_show(bg);

	Evas_Object *box = elm_box_add(bg);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(box);
	elm_object_content_set(bg, box);

	Evas_Object *label = elm_label_add(box);
	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
	elm_object_text_set(label, str);
	evas_object_show(label);
	elm_box_pack_end(box, label);

	return bg;
}



Evas_Object*
get_genlist_item_2text(Evas_Object *obj, const char *text, const char *subtext){

	char str[MAX_STRING_SIZE + MAX_MARKUP_SIZE] = {0,};

	Evas_Object *bg = elm_bg_add(obj);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_min_set(bg, 100, 100);
	//elm_bg_color_set(bg, RAPROTO_BACKGROUND_ERROR);
	evas_object_show(bg);

	Evas_Object *box = elm_box_add(bg);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(box);
	elm_object_content_set(bg, box);

	Evas_Object *label = elm_label_add(box);
	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_main, text);
	elm_object_text_set(label, str);
	evas_object_show(label);
	elm_box_pack_end(box, label);

	Evas_Object *sublabel = elm_label_add(box);
	snprintf(str, MAX_STRING_SIZE+MAX_MARKUP_SIZE, "%s%s", font_markup_sub, subtext);
	elm_object_text_set(sublabel, str);
	evas_object_show(sublabel);
	elm_box_pack_end(box, sublabel);

	return bg;
}


void
get_background_for_state(Evas_Object *bg, service_data_s *service){
	if (service->error) elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_ERROR);
	else if (service->logging) elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_LOGGING);
	else elm_bg_color_set(bg, RAPROTO_COLOR_GENLIST_DEFAULT);
}




/*
 * Window delete request callback
 */
Eina_Bool
win_delete_request_cb(void *data, Elm_Object_Item *it) {
	/* Terminate the Sync Adapter sample */
	ui_app_exit();
	return EINA_TRUE;
}



void
create_text_display(app_data_s *ad, const char *msg) {

	Evas_Object *nf = ad->nf, *label, *box, *scroller, *circle_scroller;
	Elm_Object_Item *nf_it;

	// Create scroller
    scroller = elm_scroller_add(nf);
    circle_scroller = eext_circle_object_scroller_add(scroller, ad->circle_surface);
	eext_circle_object_scroller_policy_set(circle_scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(circle_scroller, EINA_TRUE);
    evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL,  EVAS_HINT_FILL);
    evas_object_show(scroller);

	// Create a Box
	box = elm_box_add(scroller);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_content_set(scroller,box);
	evas_object_show(box);

    // Add label
	label = elm_label_add(box);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_padding_set(label, 50, 50, 50, 240);
	elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_label_wrap_width_set(label,240);
	elm_object_text_set(label, msg);
    elm_box_pack_end(box, label);
    evas_object_show(label);

	nf_it = elm_naviframe_item_push(nf, "Raproto", NULL, NULL, scroller, "empty");

}



void
create_entry(app_data_s *ad, Elm_Input_Panel_Layout layout, char *initial_entry, Evas_Smart_Cb callback){

	//TODO: fix needing to click back twice to exit from the entry widget (no good solution yet)

    // Create a Box
    Evas_Object *box;
    box = elm_box_add(ad->nf);
    evas_object_show(box);

    // Create a keyboard (entry) and add to box
    Evas_Object *entry;
    entry = elm_entry_add(box);

	elm_entry_entry_set(entry, initial_entry); // set this to the current ID
    elm_entry_cursor_pos_set(entry,strlen(initial_entry));
    elm_entry_single_line_set(entry, EINA_TRUE);
    elm_entry_scrollable_set(entry, EINA_TRUE);
    elm_entry_input_panel_return_key_disabled_set(entry, EINA_FALSE);
    elm_entry_input_panel_layout_set(entry, layout);
    elm_entry_input_panel_show(entry);

    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL,  EVAS_HINT_FILL); // 0.5
    evas_object_size_hint_padding_set(entry, 50, 50, 30, 240);
    evas_object_show(entry);

    elm_box_pack_end(box, entry);

    evas_object_smart_callback_add(entry, "activated", callback, ad);

	elm_naviframe_item_push(ad->nf, "Enter ID", NULL, NULL, box, "empty");

}




Evas_Object*
create_frame(frame_data_s *frame, menu_data_s *md, app_data_s *ad){
	//create_genlist(Evas_Object **genlist, menu_data_s *md, app_data_s *ad){

	Evas_Object *circle_genlist;
	list_item_data_s *lid;

	/* Elm_Genlist_Item_Class for title, on/off, list, padding */
	Elm_Genlist_Item_Class *ttc = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class *ptc = elm_genlist_item_class_new();

	frame->bg = elm_bg_add(ad->nf);

    frame->gl = elm_genlist_add(frame->bg);
	elm_genlist_mode_set(frame->gl, ELM_LIST_SCROLL);

	// pick the right background for the service state
	if (ad->service.error) {
		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_ERROR);
		elm_object_style_set(frame->gl, "transparent");
	} else if (ad->service.logging) {
		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_LOGGING);
		elm_object_style_set(frame->gl, "transparent");
	} else {
		elm_bg_color_set(frame->bg,RAPROTO_COLOR_BACKGROUND_DEFAULT);
		elm_object_style_set(frame->gl, "default");
	}

	/* genlist for circular view */
	circle_genlist = eext_circle_object_genlist_add(frame->gl, ad->circle_surface);
	//eext_circle_object_color_set(circle_genlist,255,0,0,255); \\TODO: Pick a Color Scheme for title and scroller
	eext_circle_object_genlist_scroller_policy_set(circle_genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(circle_genlist, EINA_TRUE);


	ttc->item_style = "title";
	ttc->func.text_get = md->title_text;
	ttc->func.del = gl_del_cb;

	itc->item_style = "full";
	itc->func.content_get = md->item_content;
	itc->func.del = gl_del_cb;

	ptc->item_style = "padding";

	/* Append genlist item for title */
	lid = calloc(1,sizeof(list_item_data_s));
	lid->index = -1;
	lid->settings = ad->settings;
	lid->service = &(ad->service);
	elm_genlist_item_append(frame->gl, ttc, lid, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);

	/* This logic is for appending genlist items */
	for (int idx = 0; idx < md->num_items; idx++) {

		lid = calloc(1,sizeof(list_item_data_s));
		lid->index = idx;
		lid->settings = ad->settings;
		lid->service = &(ad->service);

		elm_genlist_item_append(frame->gl, itc, lid, NULL, ELM_GENLIST_ITEM_NONE, md->func[idx], ad);
	}

	/* Append 2 genlist items for padding */
	elm_genlist_item_append(frame->gl, ptc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append(frame->gl, ptc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);

	elm_genlist_item_class_free(ttc);
	elm_genlist_item_class_free(itc);
	elm_genlist_item_class_free(ptc);

	evas_object_show(frame->gl);
	elm_object_content_set(frame->bg, frame->gl);

	return frame->bg;
}




