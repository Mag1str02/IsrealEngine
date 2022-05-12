#include "initializer.h"

namespace DE
{

    Initializer::Initializer()
    {
        _max_entity_amount = 5000;
        _max_component_amount = 128;
    }

    Initializer &Initializer::Get()
    {
        static Initializer initializer;
        return initializer;
    }
    void Initializer::SetHint(uint64_t hint_id, int64_t hint_value)
    {
        switch (hint_id)
        {
        case H_MAX_ENTITY_AMOUNT:
            _max_entity_amount = hint_value;
            break;
        case H_MAX_COMPONENT_AMOUNT:
            _max_component_amount = hint_value;
            break;
        }
    }

    void deHint(uint64_t hint_id, int64_t hint_value)
    {
        Initializer::Get().SetHint(hint_id, hint_value);
    }
} // namespace DE
