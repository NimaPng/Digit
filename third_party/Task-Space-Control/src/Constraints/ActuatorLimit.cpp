//
// Created by nimapng on 6/29/21.
//

#include "Constraints/ActuatorLimit.h"

using namespace TSC;

ActuatorLimit::ActuatorLimit(RobotWrapper &robot, string name) : LinearConstraints(robot, name, false) {
    _lb = -robot.actuatorsEffortLimit();
    _ub = robot.actuatorsEffortLimit();
}

void ActuatorLimit::update() {
    if (_C.rows() != _robot.na() || _C.cols() != (_robot.nv() + 3 * _robot.nc())) {
        _C.resize(_robot.na(), _robot.nv() + 3 * _robot.nc());
    }
    ConstMatRef Ma = _robot.M().bottomRows(_robot.na());
    ConstVecRef ba = _robot.nonLinearEffects().tail(_robot.na());
    ConstMatRef Jca = _robot.contactJacobia().rightCols(_robot.na());
    _C << Ma, -Jca.transpose();
    _c_lb = _lb - ba;
    _c_ub = _ub - ba;
}

ConstMatRef ActuatorLimit::C() {
    return TSC::ConstMatRef(_C);
}

ConstVecRef ActuatorLimit::c_lb() {
    return TSC::ConstVecRef(_c_lb);
}

ConstVecRef ActuatorLimit::c_ub() {
    return TSC::ConstVecRef(_c_ub);
}

VecRef ActuatorLimit::lb() {
    if (_lb.size() != robot().na()) {
        _lb.resize(robot().na());
    }
    return VecRef(_lb);
}

VecRef ActuatorLimit::ub() {
    if (_ub.size() != robot().na()) {
        _ub.resize(robot().na());
    }
    return VecRef(_ub);
}