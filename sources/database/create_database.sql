CREATE TABLE "orders"
(
  "order-id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "customer-name" TEXT,
  "placed-on-date-time" TEXT,
  "total" INTEGER
);

CREATE TABLE "order-items"
(
  "order-item-id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "order-id" INTEGER INTEGER REFERENCES "orders" ("order-id"),
  "name" TEXT,
  "quantity" REAL
);

CREATE INDEX "order-items-order-id" ON "order-items" ("order-id");
