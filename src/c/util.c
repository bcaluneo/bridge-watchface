#include "util.h"

BatteryChargeState get_current_battery_info() {
  return battery_state_service_peek();
}

int get_current_charge_percent() {
    BatteryChargeState battery_info = battery_state_service_peek();
    if (battery_info.is_charging) return 100; 
    
    return battery_info.charge_percent;
}

int get_current_step_count() {
  HealthMetric step_count_metric = HealthMetricStepCount;
  time_t then = time_start_of_today();
  time_t now = time(NULL);

  if (!(health_service_metric_accessible(step_count_metric, then, now) & HealthServiceAccessibilityMaskAvailable)) return -1;

  return (int) health_service_sum_today(step_count_metric);
}

int get_length_of_month(int month) {
  switch (month) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11:
      return 31;
    case 3:
    case 5:
    case 8:
    case 10:
      return 30;
    case 1:
      return 28;
    default:
      return -1;
  }
}