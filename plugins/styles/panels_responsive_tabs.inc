<?php


/**
 * @file
 * Definition of the 'Responsive Tabs' panel style.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Responsive Tabs'),
  'description' => t('Show panel panes in a region as responsive tabs.'),
  'render region' => 'panels_responsive_tabs_style_render_region',
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_panels_responsive_tabs_style_render_region($vars) {
  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $owner_id = $vars['owner_id'];
  $panes = $vars['panes'];

  $tab_id = 'tabs-' . $owner_id . '-' . $region_id;

  $element = array(
    '#prefix' => '<div id="' . $tab_id . '">',
    '#suffix' => '</div>',
    '#attached' => array(
      'library' => array(array('panels_responsive_tabs', 'easy-responsive-tabs')),
      'js' => array(
        drupal_get_path('module', 'panels_responsive_tabs') . '/js/panels_responsive_tabs.js' => array('type' => 'file'),
      ),
    ),
  );

  $settings = array(
    array(
      'identifier' => $tab_id,
      'type' => 'default',
      'width' => 'auto',
      'fit' => TRUE,
      'closed' => FALSE,
      'tabidentify' => '',
      'activetab_bg' => '#B5AC5F',
      'inactive_bg' => '#E0D78C',
      'active_border_color' => '#9C905C',
      'active_content_border_color' => '#9C905C',
    ),
  );
  $settings['panels_responsive_tabs'] = $settings;

  $element['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => $settings,
  );

  // Get the pane titles.
  $items = array();
  $delta = 1;

  if (isset($display->panels[$region_id])) {
    foreach ($display->panels[$region_id] as $pane_id) {
      // Make sure the pane exists.
      if (!empty($panes[$pane_id])) {
        $title = panels_responsive_tabs_pane_titles($display->did, $pane_id);
        $title = $title ? $title : t('Tab @delta', array('@delta' => $delta));
        $items[] = array(
          'data' => $title,
          'class' => array('tabs', 'tab--' . drupal_html_class($title)),
        );
        ++$delta;
      }
    }
  }

  if ($delta === 1) {
    // No tabs to show, the tabs wrapper must not be rendered.
    return '';
  }

  $element['tabs_title'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array(
      'class' => array('resp-tabs-list'),
    ),
  );

  $items = array();
  $delta = 1;
  $element['tabs_content'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('resp-tabs-container'),
    ),
  );
  foreach ($panes as $pane_id => $item) {
    $element['tabs_content'][$pane_id] = array(
      '#prefix' => '<div id="' . $tab_id . '-' . $delta . '">',
      '#suffix' => '</div>',
      '#markup' => $item,
    );
    ++$delta;
  }

  return drupal_render($element);
}
