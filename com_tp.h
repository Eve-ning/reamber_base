#ifndef COM_TP_H
#define COM_TP_H

#include <QtCore>
#include <QLineEdit>

class  cOM_TP
{
public:
    // CONSTRUCTORS
    cOM_TP();
    cOM_TP(QString TP);
    cOM_TP(QLineEdit *line);

    // LOADERS
    void loadTP(QString TP);
    void loadTP(QLineEdit *line);

    // SETTERS
    void setOffset        (double          newOffset        ){ offset         = newOffset        ; return; }
    void setCode          (double          newCode          ){ code           = newCode          ; return; }
    void setMetronome     (unsigned short  newMetronome     ){ metronome      = newMetronome     ; return; }
    void setSampleSet     (unsigned short  newSampleSet     ){ sampleSet      = newSampleSet     ; return; }
    void setSampleSetIndex(unsigned short  newSampleSetIndex){ sampleSetIndex = newSampleSetIndex; return; }
    void setVolume        (unsigned short  newVolume        ){ volume         = newVolume        ; return; }
    void setIsBPM         (bool            newIsBPM         ){ isBPM          = newIsBPM         ; return; }
    void setIsKiai        (bool            newIsKiai        ){ isKiai         = newIsKiai        ; return; }
    void setValue         (double newValue) ;

    // GETTERS
    void            getInfo          () const;
    double          getOffset        () const { return offset        ; }
    double          getCode          () const { return code          ; }
    unsigned short  getMetronome     () const { return metronome     ; }
    unsigned short  getSampleSet     () const { return sampleSet     ; }
    unsigned short  getSampleSetIndex() const { return sampleSetIndex; }
    unsigned short  getVolume        () const { return volume        ; }
    bool            getIsBPM         () const { return isBPM         ; }
    bool            getIsKiai        () const { return isKiai        ; }
    bool            getLoadFail      () const { return loadFail; }
    double          getValue         () const;
    QString         toString         () const;

    // OPERS
    bool operator <(cOM_TP *OM_TP) { return offset < OM_TP->getOffset(); }
    bool operator >(cOM_TP *OM_TP) { return offset > OM_TP->getOffset(); }
    bool operator <(cOM_TP  OM_TP) { return offset < OM_TP.getOffset(); }
    bool operator >(cOM_TP  OM_TP) { return offset > OM_TP.getOffset(); }

    // MISC
    void limitValue();

    static bool isTP_SV(QString TP) ;
    static bool isTP_BPM(QString TP);
    static bool isTP(QString TP)    ;



protected:
    double          offset        ;
    double          code          ;
    unsigned short  metronome     ;
    unsigned short  sampleSet     ;
    unsigned short  sampleSetIndex;
    unsigned short  volume        ;
    bool            isBPM         ;
    bool            isKiai        ;
    bool            loadFail      ;

};

#endif // COM_TP_H
