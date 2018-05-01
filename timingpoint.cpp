#include "timingpoint.h"

// CONSTRUCTORS
TimingPoint::TimingPoint()
{
    offset         = 0      ;
    code           = 100    ;
    metronome      = 4      ;
    sampleSet      = 0      ;
    sampleSetIndex = 0      ;
    volume         = 5      ;
    isBPM          = false  ;
    isKiai         = false  ;
    loadFail       = false  ;
}
TimingPoint::TimingPoint(QString newString) : TimingPoint()
{
    loadTP(newString);
}
TimingPoint::TimingPoint(QLineEdit *line) : TimingPoint()
{
    loadTP(line);
}
TimingPoint::TimingPoint(double newOffset, double newValue) : TimingPoint()
{
    loadTP(newOffset, newValue);
}

// LOADERS
void TimingPoint::loadTP(QString TP)
{
    omInfo info;

    info = amberCommon::whatOM_Type(TP);

    if (!info.getIsTP()) // Case: Invalid
    {
        loadFail = true;
        throw TPLoadFail(QString("Input not TP: ") + TP);
    }

    //            [0] [1]              [2][3][4][5][6][7]
    // REFERENCE: 638,231.660231660231,4, 1, 0, 5, 1, 0

    QStringList TP_splitComma;

    TP_splitComma = TP.split("," , QString::KeepEmptyParts);

    if (TP_splitComma.size() == 8)
    {
        offset         = TP_splitComma[0].toDouble();
        code           = TP_splitComma[1].toDouble();
        metronome      = TP_splitComma[2].toInt();
        sampleSet      = TP_splitComma[3].toInt();
        sampleSetIndex = TP_splitComma[4].toInt();
        volume         = TP_splitComma[5].toInt();
        isBPM          = (TP_splitComma[6] == "1");
        isKiai         = (TP_splitComma[7] == "1");
    }
    else
    {
        loadFail = true;
        throw amberException("An unexpected error has occured.");
    }
}
void TimingPoint::loadTP(QLineEdit *line)
{
    QString lineText;

    lineText = line->text();

    loadTP(lineText);

}
void TimingPoint::loadTP(double newOffset, double newValue)
{
    offset = newOffset;
    setValue(newValue);
}

void TimingPoint::setOffset(double newOffset)
{
    amberCommon::assertOffsetValid(newOffset);
    offset = newOffset;
}

// SETTERS
void TimingPoint::setValue(double newValue)
{
    amberCommon::assertDivByZero(newValue);

    // This will indirectly set code instead
    if (isBPM)
    {
        code = 60000 / newValue;
    }
    else
    {
        code = -100 / newValue;
    }
    return;
}

// GETTERS
void    TimingPoint::getInfo () const
{
    qDebug() << "\r\n"
             << "[---- Timing Point Info ----]"       << "\r\n"
             << "OFFSET         : " << offset         << "\r\n"
             << "CODE           : " << code           << "\r\n"
             << "METRONOME      : " << metronome      << "\r\n"
             << "SAMPLESET      : " << sampleSet      << "\r\n"
             << "SAMPLESETINDEX : " << sampleSetIndex << "\r\n"
             << "VOLUME         : " << volume         << "\r\n"
             << "ISBPM          : " << isBPM          << "\r\n"
             << "ISKIAI         : " << isKiai         << "\r\n";
}
double  TimingPoint::getValue() const
{
    double output;
    if (isBPM)
    {
        output = 60000 / code;
    }
    else
    {
        output = -100 / code;
    }
    return output;
}
QString TimingPoint::toString() const
{
    return      QString::number(offset        ) + ","
            +   QString::number(code          ) + ","
            +   QString::number(metronome     ) + ","
            +   QString::number(sampleSet     ) + ","
            +   QString::number(sampleSetIndex) + ","
            +   QString::number(volume        ) + ","
            +   (isBPM  ? "1" : "0")            + ","
            +   (isKiai ? "1" : "0") ;
}

// OPERS
void TimingPoint::multiplyValue(const TimingPoint rhsTimingPoint, bool limitFlag)
{
    setValue(getValue() * rhsTimingPoint.getValue());
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::divideValue(const TimingPoint rhsTimingPoint, bool limitFlag)
{
    amberCommon::assertDivByZero(rhsTimingPoint.getValue());

    setValue(getValue() / rhsTimingPoint.getValue());
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::addValue(const TimingPoint rhsTimingPoint, bool limitFlag)
{
    setValue(getValue() + rhsTimingPoint.getValue());
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::subtractValue(const TimingPoint rhsTimingPoint, bool limitFlag)
{
    setValue(getValue() - rhsTimingPoint.getValue());
    if (limitFlag)
    {
        limitValues();
    }
}

void TimingPoint::multiplyValue  (const double rhsDouble, bool limitFlag)
{
    setValue(getValue() * rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::divideValue    (const double rhsDouble, bool limitFlag)
{
    amberCommon::assertDivByZero(rhsDouble);

    setValue(getValue() / rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::addValue       (const double rhsDouble, bool limitFlag)
{
    setValue(getValue() + rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::subtractValue  (const double rhsDouble, bool limitFlag)
{
    setValue(getValue() - rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}

void TimingPoint::multiplyOffset (const double rhsDouble, bool limitFlag)
{
    setOffset(getOffset() * rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::divideOffset   (const double rhsDouble, bool limitFlag)
{
    amberCommon::assertDivByZero(rhsDouble);

    setOffset(getOffset() * rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::addOffset      (const double rhsDouble, bool limitFlag)
{
    setOffset(getOffset() * rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}
void TimingPoint::subtractOffset (const double rhsDouble, bool limitFlag)
{
    setOffset(getOffset() * rhsDouble);
    if (limitFlag)
    {
        limitValues();
    }
}

// MISC
void TimingPoint::limitValues(double maxSV, double minSV, double maxBPM, double minBPM)
{
    // Make sure min < max
    if ((maxSV != 0 && minSV != 0) && (minSV > maxSV))
    {
        throw amberException(QString(__FUNCTION__) + "minSV > maxSV: " + minSV + " > " + maxSV);
    }

    if ((maxBPM != 0 && minBPM != 0) && (minBPM > maxBPM))
    {
        throw amberException(QString(__FUNCTION__) + "minBPM > maxBPM: " + minBPM + " > " + maxBPM);
    }

    // If any value is exactly 0, we take it that the user doesn't want to limit it.
    if (isBPM)
    {
        // BOUND 0 ~ infinity
        if ((minBPM != 0) && (getValue() < minBPM))
        {
            setValue(minBPM);
        }
        else if ((maxBPM != 0) && getValue() > maxBPM)
        {
            setValue(maxBPM);
        }
    } else
    { //isSV
        // BOUND 0.1 ~ 10.0
        if ((minSV != 0) && getValue() < minSV)
        {
            setValue(minSV);
        }
        else if ((maxBPM != 0) && getValue() > maxSV)
        {
            setValue(maxSV);
        }
    }
}

void TimingPoint::limitOffset(double minOffset, double maxOffset)
{
    if (offset > maxOffset)
    {
        setOffset(minOffset);
    }
    else if (offset < minOffset)
    {
        setOffset(minOffset);
    }
}



