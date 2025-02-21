.. _lv_tileview:

=======================
Tile View (lv_tileview)
=======================

Overview
********

The Tile view is a container Widget whose elements (called *tiles*) can
be arranged in grid form. A user can navigate between the tiles by
swiping. Any direction of swiping can be disabled on the tiles
individually to not allow moving from one tile to another.

If the Tile view is screen sized, the user interface resembles what you
may have seen on smartwatches.

.. _lv_tileview_parts_and_styles:

Parts and Styles
****************

The Tile view is built from an :ref:`base_widget` container and
:ref:`base_widget` tiles.

The parts and styles work the same as for :ref:`base_widget`.

.. _lv_tileview_usage:

Usage
*****

Add a tile
----------

:cpp:expr:`lv_tileview_add_tile(tileview, col_id, row_id, dir)` creates a new
tile on the ``col_id``\ th column and ``row_id``\ th row. ``dir`` can be
``LV_DIR_LEFT/RIGHT/TOP/BOTTOM/HOR/VER/ALL`` or OR-ed values to enable
moving to the adjacent tiles into the given direction by swiping.

The returned value is an ``lv_obj_t *`` on which the content of the tab
can be created.

Change tile
-----------

The Tile view can scroll to a tile with
:cpp:expr:`lv_tileview_set_tile(tileview, tile_obj, LV_ANIM_ON/OFF)` or
:cpp:expr:`lv_tileview_set_tile_by_index(tileview, col_id, row_id, LV_ANIM_ON/OFF)`



.. _lv_tileview_events:

Events
******

-  :cpp:enumerator:`LV_EVENT_VALUE_CHANGED` Sent when a new tile loaded by scrolling.
   :cpp:expr:`lv_tileview_get_tile_active(tabview)` can be used to get current
   tile.

.. admonition::  Further Reading

    Learn more about :ref:`lv_obj_events` emitted by all Widgets.

    Learn more about :ref:`events`.



.. _lv_tileview_keys:

Keys
****

No *Keys* are processed by Tileview Widgets.

.. admonition::  Further Reading

    Learn more about :ref:`indev_keys`.



.. _lv_tileview_example:

Example
*******

.. include:: ../../examples/widgets/tileview/index.rst



.. _lv_tileview_api:

API
***
