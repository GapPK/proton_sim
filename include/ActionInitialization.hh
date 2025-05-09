#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization() = default;

    void BuildForMaster() const override;
    void Build() const override;
};

#endif