#ifndef FLINT_SUB_VIEWPORT_CONTAINER_H
#define FLINT_SUB_VIEWPORT_CONTAINER_H

#include "control.h"
#include "../sub_viewport.h"

namespace Flint {
    class SubViewportContainer : public Control {
    public:
        SubViewportContainer();

        void set_viewport(std::shared_ptr<SubViewport> p_viewport);

        /// Intercept viewport searching upwards the scene tree.
        //std::shared_ptr<SubViewport> get_viewport() override;

    protected:
        std::shared_ptr<SubViewport> viewport;

    protected:
        void update(double delta) override;

        void draw() override;
    };
}

#endif //FLINT_SUB_VIEWPORT_CONTAINER_H
