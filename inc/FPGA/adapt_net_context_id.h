#pragma once

typedef struct _adapt_net_context_id {
	int unit_id;
	int context_id;
	_adapt_net_context_id() {};
	_adapt_net_context_id(int uid, int cid) : unit_id(uid), context_id(cid) {}
}adapt_net_context_id;
