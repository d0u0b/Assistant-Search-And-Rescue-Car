module.exports = {
    IMG: {
        HEIGHT: 480,
        WIDTH: 640
    },
    attitudeOffset: {
        "acc-x": 4,
        "acc-y": 6,
        "acc-z": 8,
        "gyr-x": 22,
        "gyr-y": 24,
        "gyr-z": 26,
    },
    CMD: {
        GET_ATTITUDE: 0x82,
        GET_ULTRASONIC: 0x83,
        GET_MOTOR: 0x85,
        GET_IP: 0x91,
        PERSON_FINDED: 0x92,
        GET_IMG: 0x93,
        CONTINUE_FIND: 0xA1,
        STOP_FIND: 0xA2,
        RESTART: 0xA3
    }
}