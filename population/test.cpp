
#include <iostream>
#include <vector>

#include "uniself/renum.hpp"
#include "uniself/strings.hpp"
#include "uniself/math.hpp"
#include "uniself/population.hpp"

#include <gtest/gtest.h>

namespace uns::tests {

    class unit : public ::uns::population::unit<float, float> {
    private:
        float m_rate = 0.0F;
        points_type m_pregnancy = 0.0F;
    public:
        health_type health = 1.0F;
    public:
        virtual void add_points(const points_type& points_gain) override { m_pregnancy += points_gain; };
        virtual void set_damage(const health_type& health_decrease) override { health -= health_decrease; };
        virtual bool is_alive() const override { return ::uns::math::more(health, 0.0F); };
        virtual points_type points() const override { return m_pregnancy; };
    public:
        void condition() { add_points(0.40); };
    };


    using order_element = ::uns::population::order_element<
        ::uns::tests::unit
        , ::uns::population::unordered_order<::uns::tests::unit>
    >;


    class ration_source : public ::uns::population::ration_source<::uns::tests::order_element> {
    private:
        float m_stock = 0.0F;
    public:
        void refresh(float set_stock) {
            if (::uns::math::lesseq(m_stock, 0.0F)) {
                m_stock = set_stock;
            }
            else {
                m_stock += set_stock;
            };
        };
        void feed(order_element& order_element) override {
            constexpr float meal = 1.0F;

            if (::uns::math::more(m_stock - meal, -meal)) {
                order_element.unit().set_damage(-meal);
                m_stock -= meal;
            };
        };
    };


    class population_machine_with_events_check: public ::uns::population::machine<
        typename ::uns::tests::order_element::order_type,
        ::uns::tests::ration_source,
        ::uns::population::breed_manager<::uns::tests::order_element>,
        ::uns::population::fatal_act_operator<::uns::tests::order_element>,
        ::uns::population::population_size_controller
    > {
    private:
        using base = ::uns::population::machine<
            typename ::uns::tests::order_element::order_type,
            ::uns::tests::ration_source,
            ::uns::population::breed_manager<::uns::tests::order_element>,
            ::uns::population::fatal_act_operator<::uns::tests::order_element>,
            ::uns::population::population_size_controller
        >;
    public:
        UNS_RENUM(PHASE, int
            , (CONDITION_START, )
            , (CONDITION_UNIT_START, )
            , (CONDITION_UNIT_FINISH, )
            , (BEFORE_BREED, )
            , (CONDITION_FINISH, )
            , (SANITATION_START, )
            , (SANITATION_UNIT_START, )
            , (SANITATION_UNIT_FINISH, )
            , (SANITATION_FINISH, )
        );
    private:
        PHASE m_phase = PHASE::CONDITION_START;
        ::std::size_t m_unit_counter = 0;
    public:
        population_machine_with_events_check(
            int random_seed
            , base::health_type life_decrease_per_iteration
        ) :
            base{ random_seed, life_decrease_per_iteration }
        {};
    protected:
        virtual void OnConditionStart() override {
            EXPECT_EQ(
                m_phase 
                , PHASE::CONDITION_START
            );

            m_unit_counter = 0;
            m_phase = PHASE::CONDITION_UNIT_START;
        };
        virtual void OnConditionUnitStart(order_element_type& Unit) override {
            EXPECT_EQ(
                m_phase
                , PHASE::CONDITION_UNIT_START
            );

            EXPECT_LT(
                Unit.index()
                , base::units.size()
            );
            EXPECT_EQ(
                m_unit_counter
                , Unit.index()
            );

            m_phase = PHASE::CONDITION_UNIT_FINISH;
        };
        virtual void OnConditionUnitFinish(order_element_type& Unit) override {
            EXPECT_EQ(
                m_phase
                , PHASE::CONDITION_UNIT_FINISH
            );

            EXPECT_LT(
                Unit.index()
                , base::units.size()
            );
            EXPECT_EQ(
                m_unit_counter
                , Unit.index()
            );

            ++m_unit_counter;
            if (m_unit_counter >= base::units.size()) {
                m_phase = PHASE::BEFORE_BREED;
            }
            else {
                m_phase = PHASE::CONDITION_UNIT_START;
            };
        };
        virtual void OnBeforeBreed() override {
            EXPECT_EQ(
                m_phase
                , PHASE::BEFORE_BREED
            );

            m_unit_counter = 0;
            m_phase = PHASE::CONDITION_FINISH;
        };
        virtual void OnConditionFinish() override {
            EXPECT_EQ(
                m_phase
                , PHASE::CONDITION_FINISH
            );

            m_unit_counter = 0;
            m_phase = PHASE::SANITATION_START;
        };
        virtual void OnSanitationStart() override {
            EXPECT_EQ(
                m_phase
                , PHASE::SANITATION_START
            );

            m_unit_counter = 0;
            m_phase = PHASE::SANITATION_UNIT_START;
        };
        virtual void OnSanitationUnitStart(order_element_type& Unit) override {
            EXPECT_EQ(
                m_phase
                , PHASE::SANITATION_UNIT_START
            );

            EXPECT_LT(
                Unit.index()
                , base::units.size()
            );
            EXPECT_EQ(
                m_unit_counter
                , Unit.index()
            );

            m_phase = PHASE::SANITATION_UNIT_FINISH;
        };
        virtual void OnSanitationUnitFinish(order_element_type& Unit, bool UnitDoesSurvive) override {
            EXPECT_EQ(
                m_phase
                , PHASE::SANITATION_UNIT_FINISH
            );

            EXPECT_LT(
                Unit.index()
                , base::units.size()
            );
            EXPECT_EQ(
                m_unit_counter
                , Unit.index()
            );
            EXPECT_EQ(
                UnitDoesSurvive
                , Unit.unit().is_alive()
            );

            ++m_unit_counter;
            if (m_unit_counter >= base::units.size()) {
                m_phase = PHASE::SANITATION_FINISH;
            }
            else {
                m_phase = PHASE::SANITATION_UNIT_START;
            };
        };
        virtual void OnSanitationFinish() override {
            units.indexate();

            EXPECT_EQ(
                m_phase
                , PHASE::SANITATION_FINISH
            );

            m_unit_counter = 0;
            m_phase = PHASE::CONDITION_START;
        };
    };
};


template<>
::std::string testing::PrintToString(const ::uns::tests::population_machine_with_events_check::PHASE& Phase) {
    return "PHASE::" + ::uns::string::cast<::std::string>(Phase);
};


class PopulationSize : public ::testing::Test {
private:
    ::std::vector<::std::size_t> m_population_size_predictions;
private:
    void SetUp() {
        m_population_size_predictions.resize(50);

        m_population_size_predictions[0] = 1;
        m_population_size_predictions[1] = 1;
        m_population_size_predictions[2] = 2;
        m_population_size_predictions[3] = 2;
        m_population_size_predictions[4] = 3;
        m_population_size_predictions[5] = 4;
        m_population_size_predictions[6] = 4;
        m_population_size_predictions[7] = 7;
        m_population_size_predictions[8] = 8;
        m_population_size_predictions[9] = 9;
        m_population_size_predictions[10] = 14;
        m_population_size_predictions[11] = 15;
        m_population_size_predictions[12] = 21;
        m_population_size_predictions[13] = 28;
        m_population_size_predictions[14] = 31;
        m_population_size_predictions[15] = 46;
        m_population_size_predictions[16] = 55;
        m_population_size_predictions[17] = 68;
        m_population_size_predictions[18] = 96;
        m_population_size_predictions[19] = 110;
        m_population_size_predictions[20] = 149;
        m_population_size_predictions[21] = 194;
        m_population_size_predictions[22] = 230;
        m_population_size_predictions[23] = 319;
        m_population_size_predictions[24] = 390;
        m_population_size_predictions[25] = 493;
        m_population_size_predictions[26] = 664;
        m_population_size_predictions[27] = 633;
        m_population_size_predictions[28] = 359;
        m_population_size_predictions[29] = 111;
        m_population_size_predictions[30] = 100;
        m_population_size_predictions[31] = 129;
        m_population_size_predictions[32] = 116;
        m_population_size_predictions[33] = 173;
        m_population_size_predictions[34] = 127;
        m_population_size_predictions[35] = 144;
        m_population_size_predictions[36] = 140;
        m_population_size_predictions[37] = 116;
        m_population_size_predictions[38] = 144;
        m_population_size_predictions[39] = 111;
        m_population_size_predictions[40] = 100;
        m_population_size_predictions[41] = 129;
        m_population_size_predictions[42] = 116;
        m_population_size_predictions[43] = 173;
        m_population_size_predictions[44] = 127;
        m_population_size_predictions[45] = 144;
        m_population_size_predictions[46] = 140;
        m_population_size_predictions[47] = 116;
        m_population_size_predictions[48] = 144;
        m_population_size_predictions[49] = 111;

        ASSERT_EQ(
            m_population_size_predictions.size()
            , iterations_total()
        );
    };
public:
    ::std::size_t iterations_total() {
        return 50;
    };
    ::std::size_t expected_population_size(::std::size_t iteration) {
        return m_population_size_predictions.at(iteration);
    };
    float ration_refresh(::std::size_t iteration) {
        return 100;
    };
};


TEST_F(PopulationSize, EventsSequence) {
    ::uns::tests::population_machine_with_events_check pop(0, 1.1F);

    pop.units.push(::std::make_shared<::uns::tests::unit>());

    for (::std::size_t iteration = 0; iteration < iterations_total(); iteration++) {
        pop.ration_source.refresh(ration_refresh(iteration));
        for (auto unit_ptr : pop.units) {
            unit_ptr.unit().condition();
        };
        pop.condition();

        EXPECT_EQ(
            expected_population_size(iteration)
            , pop.units.size()
        ) << "Iteration #" << iteration;
    };
};
