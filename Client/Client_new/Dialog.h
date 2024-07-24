#ifndef DIALOG_H
#define DIALOG_H

#include <QString>

class Dialog{

public:

    QString dialog_time;
    QString dialog_sender_id;
    QString dialog_content;

public:
    Dialog(const QString& time = "end", const QString& sender_id = "end", const QString& content = "end"):
        dialog_time(time), dialog_sender_id(sender_id), dialog_content(content){}
};

#endif // DIALOG_H
