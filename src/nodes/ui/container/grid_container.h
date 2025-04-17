#pragma once

#include "container.h"

namespace revector {

class GridContainer : public Container {
public:
    void adjust_layout() override;

    void calc_minimum_size() override;

    void set_separation(float new_separation);

    void set_column_number(uint32_t new_column_number);

protected:
    uint32_t col_num = 1;

    /// Separation between UI children.
    float separation = 8;

    std::vector<float> max_row_heights;
    std::vector<float> max_col_widths;
};

} // namespace revector
