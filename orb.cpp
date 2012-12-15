		orbEk# = (Vx^2 + Vy^2) / 2
        orbEp# = -1 * G * P(ORrefOBJ, 4) / ORrefD
        orbE# = orbEk# + orbEp#
        orbD# = G * P(ORrefOBJ, 4)
        IF orbD# = 0 THEN orbD# = G * 1
        L2# = (ORrefD * Vtan) ^ 2
        term2# = 2 * orbE# * L2# / (orbD# * orbD#)
        ecc = SQR(1 + term2#)
