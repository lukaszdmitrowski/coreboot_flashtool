#ifndef LOGGERS_H
#define LOGGERS_H

int libflashrom_log(fl_log_level_t log_level, const char *format, va_list vl)
{
        if (log_level != FL_MSG_INFO)
        {
                QString text;
                text.vsprintf(format, vl);
                w->active_log_out->append(text);
        }
        return 1;
}

int libbiosext_log(const char *const format, ...)
{
        int ret = 0;
        QString text;
        va_list args;

        va_start(args, format);
        text.vsprintf(format, args);
        w->active_log_out->insertPlainText(text);
        va_end(args);

        return ret;
}

int libcbfstool_log(const char *const format, ...)
{
        int ret = 0;
        QString text;
        va_list args;

        va_start(args, format);
        text.vsprintf(format, args);
        w->active_log_out->insertPlainText(text);
        va_end(args);

        return ret;
}

#endif // LOGGERS_H

