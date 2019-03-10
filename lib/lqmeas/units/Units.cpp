#include "Units.h"

namespace LQMeas {

    class UnitCodes {
    public:
        enum Vals {
            V   = 100,
            mV  = 101,
            uV  = 102,

            A   = 200,
            mA  = 201,
            uA  = 202,


            m   = 300,
            mm  = 301,
            um  = 302,


            Hz  = 400,
            KHz = 401,
            MHz = 402,
            rpm = 410,

            Ohm = 500,

            degreeC = 600,

            degree  = 700,

            sec   = 800,
            msec  = 801,
            usec  = 802,

            m_sec  = 900,
            mm_sec = 901,

            m_sec2 = 1000,
            g      = 1010,


            bit    = 1100,
            Kbit   = 1101,
            Mbit   = 1102,

            byte   = 1130,
            Kbyte  = 1131,
            Mbyte  = 1132,

            percentage = 1200
        };
    };


    class unit_V : public Unit {
    public:
        int code() const {return UnitCodes::V;}
        QString name() const {return Units::tr("V");}
        double multipler() const {return 1.;}
        const Unit *baseUnit() const {return Units::V();}
    };

    class unit_mV : public Unit {
    public:
        int code() const {return UnitCodes::mV;}
        QString name() const {return Units::tr("mV");}
        double multipler() const {return 1e-3;}
        const Unit *baseUnit() const {return Units::V();}
    };

    class unit_uV : public Unit {
    public:
        int code() const {return UnitCodes::uV;}
        QString name() const {return Units::tr("µV");}
        double multipler() const {return 1e-6;}
        const Unit *baseUnit() const {return Units::V();}
    };

    class unit_A : public Unit {
    public:
        int code() const {return UnitCodes::A;}
        QString name() const {return Units::tr("A");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::A();}
    };

    class unit_mA : public Unit {
    public:
        int code() const {return UnitCodes::mA;}
        QString name() const {return Units::tr("mA");}
        double multipler() const {return 1e-3;}
        const Unit *baseUnit() const {return Units::A();}
    };

    class unit_uA : public Unit {
    public:
        int code() const {return UnitCodes::uA;}
        QString name() const {return Units::tr("µA");}
        double multipler() const {return 1e-6;}
        const Unit *baseUnit() const {return Units::A();}
    };

    class unit_m : public Unit {
    public:
        int code() const {return UnitCodes::m;}
        QString name() const {return Units::tr("m");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::m();}
    };

    class unit_mm : public Unit {
    public:
        int code() const {return UnitCodes::mm;}
        QString name() const {return Units::tr("mm");}
        double multipler() const {return 1e-3;}
        const Unit *baseUnit() const {return Units::m();}
    };

    class unit_um : public Unit {
    public:
        int code() const {return UnitCodes::um;}
        QString name() const {return Units::tr("µm");}
        double multipler() const {return 1e-6;}
        const Unit *baseUnit() const {return Units::m();}
    };


    class unit_Hz : public Unit {
    public:
        int code() const {return UnitCodes::Hz;}
        QString name() const {return Units::tr("Hz");}
        double multipler() const {return 1.;}
        const Unit *baseUnit() const {return Units::Hz();}
    };

    class unit_KHz : public Unit {
    public:
        int code() const {return UnitCodes::KHz;}
        QString name() const {return Units::tr("KHz");}
        double multipler() const {return 1e3;}
        const Unit *baseUnit() const {return Units::Hz();}
    };

    class unit_MHz : public Unit {
    public:
        int code() const {return UnitCodes::MHz;}
        QString name() const {return Units::tr("MHz");}
        double multipler() const {return 1e6;}
        const Unit *baseUnit() const {return Units::Hz();}
    };

    class unit_RPM : public Unit {
    public:
        int code() const {return UnitCodes::rpm;}
        QString name() const {return Units::tr("rpm");}
        double multipler() const {return 1./60;}
        const Unit *baseUnit() const {return Units::Hz();}
    };



    class unit_Ohm : public Unit {
    public:
        int code() const {return UnitCodes::Ohm;}
        QString name() const {return Units::tr("Ω");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::ohm();}
    };

    class unit_degreeC : public Unit {
    public:
        int code() const {return UnitCodes::degreeC;}
        QString name() const {return Units::tr("°C");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::degreeC();}
    };

    class unit_degree : public Unit {
    public:
        int code() const {return UnitCodes::degree;}
        QString name() const {return Units::tr("°");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::degree();}
    };

    class unit_sec : public Unit {
    public:
        int code() const {return UnitCodes::sec;}
        QString name() const {return Units::tr("s");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::sec();}
    };

    class unit_msec : public Unit {
    public:
        int code() const {return UnitCodes::msec;}
        QString name() const {return Units::tr("ms");}
        double multipler() const {return 1e-3;}
        const Unit *baseUnit() const {return Units::sec();}
    };

    class unit_usec : public Unit {
    public:
        int code() const {return UnitCodes::usec;}
        QString name() const {return Units::tr("µs");}
        double multipler() const {return 1e-6;}
        const Unit *baseUnit() const {return Units::sec();}
    };


    class unit_m_sec : public Unit {
    public:
        int code() const {return UnitCodes::m_sec;}
        QString name() const {return Units::tr("m/s");}
        double multipler() const {return 1.;}
        const Unit *baseUnit() const {return Units::m_s();}
    };

    class unit_mm_sec : public Unit {
    public:
        int code() const {return UnitCodes::mm_sec;}
        QString name() const {return Units::tr("mm/s");}
        double multipler() const {return 1e-3;}
        const Unit *baseUnit() const {return Units::m_s();}
    };

    class unit_m_sec2 : public Unit {
    public:
        int code() const {return UnitCodes::m_sec2;}
        QString name() const {return Units::tr("m/s²");}
        double multipler() const {return 1.;}
        const Unit *baseUnit() const {return Units::m_s2();}
    };

    class unit_g : public Unit {
    public:
        int code() const {return UnitCodes::g;}
        QString name() const {return Units::tr("g");}
        double multipler() const {return 9.80665;}
        const Unit *baseUnit() const {return Units::m_s2();}
    };

    class unit_bit : public Unit {
    public:
        int code() const {return UnitCodes::bit;}
        QString name() const {return Units::tr("bit");}
        double multipler() const {return 1;}
        const Unit *baseUnit() const {return Units::bit();}
    };

    class unit_Kbit : public Unit {
    public:
        int code() const {return UnitCodes::Kbit;}
        QString name() const {return Units::tr("Kbit");}
        double multipler() const {return 1e3;}
        const Unit *baseUnit() const {return Units::bit();}
    };

    class unit_Mbit : public Unit {
    public:
        int code() const {return UnitCodes::Mbit;}
        QString name() const {return Units::tr("Mbit");}
        double multipler() const {return 1e6;}
        const Unit *baseUnit() const {return Units::bit();}
    };


    class unit_byte : public Unit {
    public:
        int code() const {return UnitCodes::byte;}
        QString name() const {return Units::tr("byte");}
        double multipler() const {return 8;}
        const Unit *baseUnit() const {return Units::bit();}
    };

    class unit_Kbyte : public Unit {
    public:
        int code() const {return UnitCodes::Kbyte;}
        QString name() const {return Units::tr("Kbyte");}
        double multipler() const {return 8*1e3;}
        const Unit *baseUnit() const {return Units::bit();}
    };

    class unit_Mbyte : public Unit {
    public:
        int code() const {return UnitCodes::Mbyte;}
        QString name() const {return Units::tr("Mbyte");}
        double multipler() const {return 8*1e6;}
        const Unit *baseUnit() const {return Units::bit();}
    };

    class unit_Percentage : public Unit {
    public:
        int code() const {return UnitCodes::percentage;}
        QString name() const {return Units::tr("%");}
        double multipler() const {return 1e6;}
        const Unit *baseUnit() const {return Units::percentage();}
    };






    const Unit *Units::V() {
        static const unit_V u;
        return &u;
    }

    const Unit *Units::mV() {
        static const unit_mV u;
        return &u;
    }

    const Unit *Units::uV() {
        static const unit_uV u;
        return &u;
    }

    const Unit *Units::A() {
        static const unit_A u;
        return &u;
    }

    const Unit *Units::mA() {
        static const unit_mA u;
        return &u;
    }

    const Unit *Units::uA() {
        static const unit_uA u;
        return &u;
    }

    const Unit *Units::m() {
        static const unit_m u;
        return &u;
    }

    const Unit *Units::mm() {
        static const unit_mm u;
        return &u;
    }

    const Unit *Units::um() {
        static const unit_um u;
        return &u;
    }

    const Unit *Units::rpm() {
        static const unit_RPM u;
        return &u;
    }

    const Unit *Units::Hz() {
        static const unit_Hz u;
        return &u;
    }

    const Unit *Units::KHz() {
        static const unit_KHz u;
        return &u;
    }

    const Unit *Units::MHz() {
        static const unit_MHz u;
        return &u;
    }

    const Unit *Units::ohm() {
        static const unit_Ohm u;
        return &u;
    }

    const Unit *Units::degreeC() {
        static const unit_degreeC u;
        return &u;
    }

    const Unit *Units::degree() {
        static const unit_degree u;
        return &u;
    }

    const Unit *Units::sec() {
        static const unit_sec u;
        return &u;
    }

    const Unit *Units::msec() {
        static const unit_msec u;
        return &u;
    }

    const Unit *Units::usec() {
        static const unit_usec u;
        return &u;
    }

    const Unit *Units::m_s() {
        static const unit_m_sec u;
        return &u;
    }

    const Unit *Units::mm_s() {
        static const unit_mm_sec u;
        return &u;
    }

    const Unit *Units::m_s2() {
        static const unit_m_sec2 u;
        return &u;
    }

    const Unit *Units::g() {
        static const unit_g u;
        return &u;
    }

    const Unit *Units::bit() {
        static const unit_bit u;
        return &u;
    }

    const Unit *Units::Kbit() {
        static const unit_Kbit u;
        return &u;
    }

    const Unit *Units::Mbit() {
        static const unit_Mbit u;
        return &u;
    }

    const Unit *Units::byte() {
        static const unit_byte u;
        return &u;
    }

    const Unit *Units::Kbyte() {
        static const unit_Kbyte u;
        return &u;
    }

    const Unit *Units::Mbyte() {
        static const unit_Mbyte u;
        return &u;
    }

    const Unit *Units::percentage()     {
        static const unit_Percentage u;
        return &u;
    }
}
