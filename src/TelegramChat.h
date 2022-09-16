//
// Created by Simone on 16/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_TELEGRAMCHAT_H
#define ELOQUENTSURVEILLANCE_TELEGRAMCHAT_H


#include "./TelegramChannel.h"


namespace EloquentSurveillance {
    /**
     * Syntactic sugar over Telegram channel
     */
    class TelegramChat {
    public:
        TelegramChannel bot;

        /**
         *
         * @param token
         * @param chatId
         */
        TelegramChat(const char *token, const uint64_t chatId) :
            bot(token),
            _chatId(chatId) {

        }

        /**
         * Send text message
         *
         * @param message
         * @return
         */
        bool sendMessage(String message) {
            return bot.sendMessage(_chatId, message);
        }

        /**
         * Send photo as attachment
         *
         * @return
         */
        bool sendPhoto() {
            return bot.sendPhoto(_chatId);
        }

    protected:
        const uint64_t _chatId;
    };
}

#endif //ELOQUENTSURVEILLANCE_TELEGRAMCHAT_H
