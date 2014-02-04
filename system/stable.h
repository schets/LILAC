#ifndef stable_h
#define stable_h
//!A system that attempts to reach an equilibrium
/*! This class defines a system that tries to reach an equilibrium
 * The actual implementation 
 */
class stable:public system:
{
    protected:
        double get_diff(comp* ucur, comp* u_last) = 0;
        void _iterate_system();
    public:
        double simulate();
};
#endif
