USE database1;
SELECT id, name, address FROM users WHERE is_customer IS NOT NULL ORDER BY created;
INSERT INTO user_notes (id, user_id, note, created) VALUES (1, 1, "Note 1", NOW());
DELETE FROM database2.logs WHERE id < 1000;
SELECT filed1, filed2_and_half FROM __table_weired WHERE filed1 > 10;
SELECT text_field FROM text_table WHERE text_field = "Unable to resolve host";